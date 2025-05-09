% ==============================================
% Alex Junior Guimaraes
% ==============================================

% ==============================================
% Projetando filtro digital notch de fc 60Hz

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

% Filtro Chebychev 1
%RP - ripple aceitável na banda passante - dB
%Wc - frequência de corte
%N - ordem do filtro

%RS - atenuação desejável mínima - dB
%Wp - banda passante
%Ws - banda de rejeição

RP=0.5;
fc=[58,62];
Wc=fc/(f/2);
N=2;

[b,a]=cheby1(N,RP,Wc,'stop');
[h,freq]=freqz(b,a,length(sig),f);
K0=max(abs(h));
plot(freq,20*log10(abs(h)/K0))
xlabel("Frequência (Hz)")
ylabel("Magnitude de H(jw)")

%%% Filtragem
filt=filter(b,a,sig);
ffilt=20*log10(abs(fft(filt)));
% Função flipfft() do arquivo flipfft.m foi complementamente removida do repositório.
% Código é de outra autoria e não foi consultado por uma possível permissão para divulgação.
% Futuramente será corrigido ou reescrito.
%flipfft(ffilt,f)

% funcao de transferência do filtro
H=tf(b,a,1/f)
figure
pzmap(H)
