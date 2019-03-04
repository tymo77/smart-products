%%% ANALYZE AND PLOT DATA FROM LAB 5 EXPERIMENTS
%%% Tyler Morrison
%%% Smart Products 2/27/2019

clearvars; clearvars global; close all;
% 000ALL.csv
% Ch1 - > Vs
% Ch2 - > Average V1 V2
% Ch3 - > LED 1
% Ch4 - > LED 2
% 5Hz
data1 = csvread('.\Data\tek0000ALL.csv',21,0);

% 000ALL.csv
% Ch1 - > Vs
% Ch2 - > Average V1 V2
% Ch3 - > LED 1
% Ch4 - > LED 2
% 50.7Hz
data2 = csvread('.\Data\tek0001ALL.csv',21,0);

%%
tmin = -2E-3;
tmax = 2E-3;
t1 = data1(:,1);
mask = t1 > tmin & t1 < tmax;

close all;
figure('Units','inches','Position',[.5 .5 10 5]); hold on;
plot(t1(mask),data1(mask,2))
plot(t1(mask),data1(mask,3))
plot(t1(mask),data1(mask,4))
plot(t1(mask),data1(mask,5))
ylim([-.5,5]);
xlabel('t(s)');
ylabel('V');
ca = gca;
set(ca,'FontSize',18);
grid on;
lgd = legend('V_s','V_{12}','LED1','LED2','Location','eastoutside');
lgd.NumColumns = 2;
saveas(gcf,'5hz_all','epsc')
%%
tmin = -.02E-3;
tmax = .08E-3;
t1 = data1(:,1);
mask = t1 > tmin & t1 < tmax;

close all;
figure('Units','inches','Position',[.5 .5 10 5]); hold on;
plot(t1(mask),data1(mask,2))
plot(t1(mask),data1(mask,3))
plot(t1(mask),data1(mask,4))
plot(t1(mask),data1(mask,5))
ylim([-.5,5]);
xlabel('t(s)');
ylabel('V');
ca = gca;
set(ca,'FontSize',18);
grid on;
lgd = legend('V_s','V_{12}','LED1','LED2','Location','eastoutside');
lgd.NumColumns = 2;
saveas(gcf,'5hz_zoom','epsc')
%%
tmin = -2E-3;
tmax = 2E-3;
t1 = data2(:,1);
mask = t1 > tmin & t1 < tmax;

close all;
figure('Units','inches','Position',[.5 .5 10 5]); hold on;
plot(t1(mask),data2(mask,2))
plot(t1(mask),data2(mask,3))
plot(t1(mask),data2(mask,4))
plot(t1(mask),data2(mask,5))
ylim([-.5,5]);
xlabel('t(s)');
ylabel('V');
ca = gca;
set(ca,'FontSize',18);
grid on;
lgd = legend('V_s','V_{12}','LED1','LED2','Location','eastoutside');
lgd.NumColumns = 2;
saveas(gcf,'50hz_all','epsc')

%%
tmin = -0.01E-3;
tmax = 0.01E-3;
t1 = data2(:,1);
mask = t1 > tmin & t1 < tmax;

close all;
figure('Units','inches','Position',[.5 .5 10 5]); hold on;
plot(t1(mask),data2(mask,2))
plot(t1(mask),data2(mask,3))
plot(t1(mask),data2(mask,4))
plot(t1(mask),data2(mask,5))
ylim([-.5,5]);
xlabel('t(s)');
ylabel('V');
ca = gca;
set(ca,'FontSize',18);
grid on;
tt = 3.1e-6;
xline(tt);
text(tt-15e-7, -0.65,[num2str(tt)],'FontSize',16);
lgd = legend('V_s','V_{12}','LED1','LED2','Location','eastoutside');
lgd.NumColumns = 2;
saveas(gcf,'50hz_zoom','epsc')

%%
tmin = -2E-6;
tmax = 5E-6;
t1 = data2(:,1);
mask = t1 > tmin & t1 < tmax;

close all;
figure('Units','inches','Position',[.5 .5 10 5]); hold on;
plot(t1(mask),data2(mask,2))
plot(t1(mask),data2(mask,3))
plot(t1(mask),data2(mask,4))
plot(t1(mask),data2(mask,5))
ylim([-.5,5]);
xlabel('t(s)');
ylabel('V');
ca = gca;
set(ca,'FontSize',18);
grid on;
lgd = legend('V_s','V_{12}','LED1','LED2','Location','eastoutside');
lgd.NumColumns = 2;
saveas(gcf,'50hz_zoom2','epsc')

%%
close all
tmin = -0.05E-3;
tmax = 0.05E-3;
t = data2(:,1);
mask = t > tmin & t < tmax;
windowSize = 1500;
b = (1/windowSize)*ones(1,windowSize);
a = 1;
y = filter(b,a,data2(:,2)-data2(:,3));

figure('Units','inches','Position',[.5 .5 10 5]); hold on;
plot(t(mask),data2(mask,2)-data2(mask,3))
hold on
plot(t(mask),y(mask),'LineWidth',3)
ylim([-0.2 0.2]);
xlim([tmin tmax]);
xlabel('t(s)');
ylabel('\Delta V');
ca = gca;
set(ca,'FontSize',18);
grid on;
ymask = y(mask);
tmask = t(mask);
ifirst=find(ymask>0,1,'last');
ilast =find(ymask<0,1,'first');
xline(tmask(ifirst));
% xline(tmask(ilast));
yline(ymask(ifirst));
% yline(ymask(ilast));
% rectangle('Position',[tmask(ifirst)+5e-7 -0.199 2.25e-5 0.03],'FaceColor','white')
text(tmask(ifirst)-6e-6, -0.22,[num2str(tmask(ifirst))],'FontSize',16);
lgd = legend('V_s-V_{12}','Time averaged V_s-V_{12}','Location','best');
saveas(gcf,'50hz_avg_diff','epsc')
%%
function xline(x)
line([x x],[-100,100],'Color','black','LineWidth',2);
end
function yline(y)
line([-100,100],[y y],'Color','black','LineWidth',2);
end