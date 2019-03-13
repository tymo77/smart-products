from threading import Timer
import time
import atexit
from sys import version_info


PWM = 0
WS2812 = 1

RGB = 0
GRB = 1
RGBW = 2
GRBW = 3

class PanTilt:
    """PanTilt HAT Driver

    Communicates with PanTilt HAT over i2c
    to control pan, tilt and light functions

    """
    REG_CONFIG = 0x00
    REG_SERVO1 = 0x01
    REG_SERVO2 = 0x03
    REG_WS2812 = 0x05
    REG_UPDATE = 0x4E
    UPDATE_WAIT = 0.03
    NUM_LEDS = 24

    def __init__(self,
                 enable_lights=True,
                 idle_timeout=2, # Idle timeout in seconds
                 light_mode=WS2812,
                 light_type=RGB,
                 servo1_min=575,
                 servo1_max=2325,
                 servo2_min=575,
                 servo2_max=2325,
                 address=0x15,
                 i2c_bus=None):

        self._is_setup = False

        self._idle_timeout = idle_timeout
        self._servo1_timeout = None
        self._servo2_timeout = None

        self._i2c_retries = 10
        self._i2c_retry_time = 0.01

        self._enable_servo1 = False
        self._enable_servo2 = False
        self._enable_lights = enable_lights
        self._light_on = 0

        self._servo_min = [servo1_min, servo2_min]
        self._servo_max = [servo1_max, servo2_max]

        self._light_mode = light_mode
        self._light_type = light_type

        self._i2c_address = address
        self._i2c = i2c_bus

    def setup(self):
        if self._is_setup:
            return True

        if self._i2c is None:
            try:
                from smbus import SMBus
                self._i2c = SMBus(1)
            except ImportError:
                if version_info[0] < 3:
                    raise ImportError("This library requires python-smbus\nInstall with: sudo apt-get install python-smbus")
                elif version_info[0] == 3:
                    raise ImportError("This library requires python3-smbus\nInstall with: sudo apt-get install python3-smbus")

        self.clear()
        self._set_config()
        atexit.register(self._atexit)

        self._is_setup = True

    def _atexit(self):
        if self._servo1_timeout is not None:
            self._servo1_timeout.cancel()

        if self._servo2_timeout is not None:
            self._servo2_timeout.cancel()

        self._enable_servo1 = False
        self._enable_servo2 = False

        self._set_config()

    def idle_timeout(self, value):
        """Set the idle timeout for the servos

        Configure the time, in seconds, after which the servos will be automatically disabled.

        :param value: Timeout in seconds

        """

        self._idle_timeout = value

    def _set_config(self):
        """Generate config value for PanTilt HAT and write to device."""

        config = 0
        config |= self._enable_servo1
        config |= self._enable_servo2 << 1
        config |= self._enable_lights << 2
        config |= self._light_mode    << 3
        config |= self._light_on      << 4

        self._i2c_write_byte(self.REG_CONFIG, config)

    def _check_int_range(self, value, value_min, value_max):
        """Check the type and bounds check an expected int value."""

        if type(value) is not int:
            raise TypeError("Value should be an integer")
        if value < value_min or value > value_max:
            raise ValueError("Value {value} should be between {min} and {max}".format(
                value=value,
                min=value_min,
                max=value_max))

    def _check_range(self, value, value_min, value_max):
        """Check the type and bounds check an expected int value."""

        if value < value_min or value > value_max:
            raise ValueError("Value {value} should be between {min} and {max}".format(
                value=value,
                min=value_min,
                max=value_max))

    def _servo_us_to_degrees(self, us, us_min, us_max):
        """Converts pulse time in microseconds to degrees

        :param us: Pulse time in microseconds
        :param us_min: Minimum possible pulse time in microseconds
        :param us_max: Maximum possible pulse time in microseconds

        """

        self._check_range(us, us_min, us_max)
        servo_range = us_max - us_min
        angle = (float(us - us_min) / float(servo_range)) * 180.0
        return int(round(angle, 0)) - 90

    def _servo_degrees_to_us(self, angle, us_min, us_max):
        """Converts degrees into a servo pulse time in microseconds

        :param angle: Angle in degrees from -90 to 90

        """

        self._check_range(angle, -90, 90)

        angle += 90
        servo_range = us_max - us_min
        us = (servo_range / 180.0) * angle
        return us_min + int(us)

    def _servo_range(self, servo_index):
        """Get the min and max range values for a servo"""

        return (self._servo_min[servo_index], self._servo_max[servo_index])

    def _i2c_write_word(self, reg, data):
        if type(data) is int:
            for x in range(self._i2c_retries):
                try:
                    self._i2c.write_word_data(self._i2c_address, reg, data)
                    return
                except IOError:
                    time.sleep(self._i2c_retry_time)
                    continue

            raise IOError("Failed to write word")

    def _i2c_write_byte(self, reg, data):
        if type(data) is int:
            for x in range(self._i2c_retries):
                try:
                    self._i2c.write_byte_data(self._i2c_address, reg, data)
                    return
                except IOError:
                    time.sleep(self._i2c_retry_time)
                    continue

            raise IOError("Failed to write byte")

    def _i2c_read_word(self, reg):
        for x in range(self._i2c_retries):
            try:
                return self._i2c.read_word_data(self._i2c_address, reg)
            except IOError:
                time.sleep(self._i2c_retry_time)
                continue

        raise IOError("Failed to read byte")

    def servo_enable(self, index, state):
        """Enable or disable a servo.

        Disabling a servo turns off the drive signal.

        It's good practise to do this if you don't want
        the Pan/Tilt to point in a certain direction and
        instead want to save power.

        :param index: Servo index: either 1 or 2
        :param state: Servo state: True = on, False = off

        """

        self.setup()

        if index not in [1, 2]:
            raise ValueError("Servo index must be 1 or 2")

        if state not in [True, False]:
            raise ValueError("State must be True/False")

        if index == 1:
            self._enable_servo1 = state
        else:
            self._enable_servo2 = state

        self._set_config()

    def get_servo_one(self):
        """Get position of servo 1 in degrees."""

        self.setup()

        us_min, us_max = self._servo_range(0)
        us = self._i2c_read_word(self.REG_SERVO1)

        try:
            return self._servo_us_to_degrees(us, us_min, us_max)
        except ValueError:
            return 0

    def get_servo_two(self):
        """Get position of servo 2 in degrees."""

        self.setup()

        us_min, us_max = self._servo_range(1)
        us = self._i2c_read_word(self.REG_SERVO2)
        try:
            return self._servo_us_to_degrees(us, us_min, us_max)
        except ValueError:
            return 0

    def servo_one(self, angle):
        """Set position of servo 1 in degrees.

        :param angle: Angle in degrees from -90 to 90

        """

        self.setup()

        if not self._enable_servo1:
            self._enable_servo1 = True
            self._set_config()

        us_min, us_max = self._servo_range(0)
        us = self._servo_degrees_to_us(angle, us_min, us_max)
        self._i2c_write_word(self.REG_SERVO1, us)

        if self._idle_timeout > 0:
            if self._servo1_timeout is not None:
                self._servo1_timeout.cancel()

            self._servo1_timeout = Timer(self._idle_timeout, self._servo1_stop)
            self._servo1_timeout.daemon = True
            self._servo1_timeout.start()

    def _servo1_stop(self):
        self._servo1_timeout = None
        self._enable_servo1 = False
        self._set_config()

    def servo_two(self, angle):
        """Set position of servo 2 in degrees.

        :param angle: Angle in degrees from -90 to 90

        """

        self.setup()

        if not self._enable_servo2:
            self._enable_servo2 = True
            self._set_config()

        us_min, us_max = self._servo_range(1)
        us = self._servo_degrees_to_us(angle, us_min, us_max)
        self._i2c_write_word(self.REG_SERVO2, us)

        if self._idle_timeout > 0:
            if self._servo2_timeout is not None:
                self._servo2_timeout.cancel()

            self._servo2_timeout = Timer(self._idle_timeout, self._servo2_stop)
            self._servo2_timeout.daemon = True
            self._servo2_timeout.start()

    def _servo2_stop(self):
        self._servo2_timeout = None
        self._enable_servo2 = False
        self._set_config()

    pan = servo_one
    tilt = servo_two
    get_pan = get_servo_one
    get_tilt = get_servo_two
