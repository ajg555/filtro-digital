% ==============================================
% atividade 04 - A

clc;
close all;
clear all;

% carregando pacotes necessários
pkg load control;
pkg load signal;

% freq. de amost
f = 1e3;

% criando sinal
t=0:1/f:2;
s1 = sin(2*pi*30*t);
s2 = sin(2*pi*60*t);
s3 = sin(2*pi*100*t);
sig = s1 + s2 + s3;


% DFT do sinal - amplitude
figure
fsig=abs(fft(sig));
flipfft(fsig,f)
title('DFT do sinal')

% aplicar chebychev 1
% fechar banda sobre a freq. principal

%RP - ripple aceitável na banda passante - dB
%Wc - frequência de corte
%N - ordem do filtro

%RS - atenuação desejável mínima - dB
%Wp - banda passante
%Ws - banda de rejeição

RP=20;
RS=30;
fp=[59, 61];
fs=[50, 70];

Ws=fs/(f/2);
Wp=fp/(f/2);

% calculo dos coef. do filtro
[N,Wp]=cheb2ord(Wp, Ws, RP, RS);
[b,a]=cheby2(N,RP,Wp, 'stop');
% ordem 2

% resposta em freq. do filtro
[h,freq]=freqz(b,a,length(sig),f);
K0=max(abs(h));
figure
plot(freq,20*log10(abs(h)/K0))
xlabel("Frequência (Hz)")
ylabel("Magnitude de H(jw)")

% filtragem do sinal
filt=filter(b,a,sig);
ffilt=20*log10(abs(fft(filt)));
flipfft(ffilt,f)
title('Sinal filtrado')

% funcao de transferência do filtro
H=tf(b,a,1/f)
figure
pzmap(H)