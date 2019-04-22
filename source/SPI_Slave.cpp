/*********************************************************************************************************
**********************************************************************************************************
							 SPI_Slave.cpp
**********************************************************************************************************
**********************************************************************************************************
	@author		Dylan DeSantis
	@date 		3/25/2018
	@version	1.0.0
**********************************************************************************************************/
#include "SPI_Slave.h"
#include<unistd.h>
#include <iostream>
//=====================================================================
// -------------------------------------- METH: Open/closing and utilities -------------------------------------------
//=====================================================================
int SPI_Slave::spiBegin()
{
	/**
		Function to set the slave device and modes of the SPI	
	*/
	int mode = this->spi_mode;
	if(mode == 0) ioctl(this->spi_fd, SPI_IOC_WR_MODE, SPI_MODE_0);
	if(mode == 1) ioctl(this->spi_fd, SPI_IOC_WR_MODE, SPI_MODE_1);
	if(mode == 2) ioctl(this->spi_fd, SPI_IOC_WR_MODE, SPI_MODE_2);
	if(mode == 3) ioctl(this->spi_fd, SPI_IOC_WR_MODE, SPI_MODE_3);
	
	int bpw = this->spi_bits_per_word;
	if(bpw>0) ioctl(this->spi_fd, SPI_IOC_WR_BITS_PER_WORD,bpw);
	
	int maxspd = this->spi_max_speed;
	if(maxspd>0) ioctl(this->spi_fd,SPI_IOC_WR_MAX_SPEED_HZ , maxspd);
	 return 0;
}
int SPI_Slave::spiEnd()
{
	/**
		Function to end any concurent threads (optional)
	*/
	
	
}
void SPI_Slave::spiDelay(int delay_time, timeScale mult)
{
	/**
		General function to provide a delay in the program in microseconds.
		
		@param delay_time the amount of time to delay the program in microseconds	
	*/
 	usleep((delay_time*mult));
 	return;
}
void SPI_Slave::spiPutBlkCmds( unsigned int rxlens[], unsigned int txlens[],unsigned char cmds[], int numCmds,int numXfers)
{
	/**
		Function for placing the transfer data into (-->) the deques: cmd --> stream_out, rxlens --> receive_len, txlens --> transmit_len
		
		@param rxlens an array of size numXfers, with each entry's value being the number of commands that the corresponding transfer will receive	
		@param txlens an array of size numXfers, with each entry's value being the number of commands that the corresponding transfer will transmit
		@param cmds an array of size numCmds, with each entry's value being the command with word size corresponding to the SPI bits per word attribute
		@param numCmds an integer value denoting the total number of transfers
		@param numXfers an integer value denoting the total number of transfers
	*/
	for (int i = 0; i<numCmds; i++)
	{
		this->stream_out.push_back(cmds[i]);
	} 
	for (int i = 0; i<numXfers; i++)
	{
		this->transmit_len.push_back(txlens[i]);
		this->receive_len.push_back(rxlens[i]);
	}
}
	
void SPI_Slave::spiClearQueues()
{
	/**
		Function to clear the deques: stream_in, stream_out, transmit_len, reviece_len.	
	*/
	this->stream_in.clear();
	this->stream_out.clear();
	this->transmit_len.clear();
	this->receive_len.clear();
}

//=====================================================================
// -------------------------------------- METH: Read/ Write  ----------------------------------------------------------------
//=====================================================================
void SPI_Slave::spiTransfer(int outbuf_size, int inbuf_size, xferTiming xfer_type, int speed_hz, int delay_us)
{
	/**
		Function for completing a transfer of data over SPI. 		
		
		@param outbuf_size an integer value denoting the size of the output buffer	
		@param inbuf_size an integer value denoting the size of the input buffer (receiving data) 		
		@param xfer_type an emun type value declaring if transmit and receiving messages occur synchronously or sequentially 
	*/
	/*
	The function should work in the following way:
			1. Check to see if there are any contents in the transmit_len deque.
			2. setup conditions to handle both synchronous and sequential data transmission
			3.. for each transfer (each entry in transmit_len)
				a.  fill the output buffer with the correct amount of commands.
				b. Setup a receive buffer if  there any messages to acquire
				c. transmit the message using 
					ioctl(this->spi_fd, SPI_IOC_MESSAGE(numMess), xfer)
				d. place contents of inbuf into stream_in if there is any incoming messages
				
	Find examples at: https://github.com/01org/edison-linux/blob/master/Documentation/spi/spidev_fdx.c
	Other Resources at:
	1. http://en.cppreference.com/w/cpp/string/byte/memset
	2. https://www.emcraft.com/stm32f429discovery/accessing-spi-devices-in-linux
	3. https://github.com/spotify/linux/blob/master/include/linux/spi/spidev.h
	4. https://www.kernel.org/doc/Documentation/spi/spidev
*/

	int sz =this->transmit_len.size();
	int szr = this->receive_len.size();
	if(sz ==0 && szr==0) {
		perror("No data to transmit or receive");
		return;
	}
	// iterating through transfers
	for(int i = 0;i <sz; i++)
	{
		//Checking if there if there is data to receive, which dictates the number of 
		// messages we will set for the transfer
		bool tx_only = false; // only sending messages
		bool rx_only = false; // only recieving messages
		int numMess;
		
		if (this->receive_len.front() == 0 ) 
		{
			numMess =1;
			tx_only = true;
		}
		else if(this->transmit_len.front() ==0)
		{
			numMess = 1;
			rx_only = true;
		}
		else if (xfer_type == SYNC)
		{
			numMess = 1;
			inbuf_size = outbuf_size;
		}
		else { numMess = 2;}
		
		struct spi_ioc_transfer xfer[numMess];
		unsigned char outbuf[outbuf_size],inbuf[inbuf_size];
		memset(xfer, 0, sizeof(xfer)); // allocating 0's to all of the transfers
		memset(outbuf, 0, sizeof(outbuf)); //allocating 0's to all of the output buffer
		memset(inbuf, 0, sizeof(inbuf)); // allocating 0's to all of the input buffer
		if (tx_only)
		{
			//filling the output buffer with the transmit data 
			for(int j = 0; j < this->transmit_len.front(); j++)
			{
				outbuf[j] = this->stream_out.front();
				this->stream_out.pop_front();
			}
			//assigning the structure to the output buffer
			xfer[0].tx_buf = (unsigned long)outbuf;
			xfer[0].len = this->transmit_len.front();
			xfer[0].speed_hz = speed_hz;
			xfer[0].delay_usecs  = delay_us;
			this->transmit_len.pop_front(); //remove the front length of the buffer, 
		} else if(rx_only)
		{
			//assigning the structure to the output buffer
			xfer[0].rx_buf = (unsigned long)inbuf;
			xfer[0].len = this->receive_len.front();
			xfer[0].speed_hz = speed_hz;
			xfer[0].delay_usecs  = delay_us;
			this->receive_len.pop_front(); 
		} else if (xfer_type == SYNC)
		{
			//filling the output buffer with the transmit data 
			for(int j = 0; j < this->transmit_len.front(); j++)
			{
				outbuf[j] = this->stream_out.front();
				this->stream_out.pop_front();
			}
			//assigning the structure to the output buffer
			xfer[0].tx_buf = (unsigned long)outbuf;
			xfer[0].rx_buf = (unsigned long)inbuf;
			xfer[0].len = this->transmit_len.front();
			xfer[0].speed_hz = speed_hz;
			xfer[0].delay_usecs  = delay_us;
			this->transmit_len.pop_front(); //remove the front length of the buffer, 
			this->receive_len.pop_front(); 
		} else if(numMess ==2)
		{
			//filling the output buffer with the transmit data 
			for(int j = 0; j < this->transmit_len.front(); j++)
			{
				outbuf[j] = this->stream_out.front();
				this->stream_out.pop_front();
			}
			//assigning the structure to the output buffer
			xfer[0].tx_buf = (unsigned long)outbuf;
			xfer[0].len = this->transmit_len.front();
			xfer[0].speed_hz = speed_hz;
			xfer[0].delay_usecs  = delay_us;
					
			xfer[1].rx_buf = (unsigned long)inbuf;
			xfer[1].len = this->receive_len.front();
			xfer[1].speed_hz = speed_hz;
			xfer[1].delay_usecs  = delay_us;
			
			this->transmit_len.pop_front(); //remove the front length of the buffer, 
			this->receive_len.pop_front(); 
		} else
		{
			printf("Error: No recognized protocol");
			return;
		}

		//Complete the transfers
		if(ioctl(this->spi_fd, SPI_IOC_MESSAGE(numMess), xfer) < 0) {	
			perror("Unable to send data");
			return;
		}
		if(numMess == 2)
		{
			//populate the stream_in buffer results for received data
			for(int i = 0; i<xfer[1].len; i++)
			{
				this->stream_in.push_back(inbuf[i]);
			}
		}else if(xfer_type == SYNC || rx_only)
		{
			for(int i = 0; i<xfer[0].len; i++)
			{
				this->stream_in.push_back(inbuf[i]);
			}
		}
	}
	return;
}


