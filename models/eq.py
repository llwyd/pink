# cross over filter experiments

import matplotlib.pyplot as plt
import numpy as np
from scipy import signal

def fft(x,fs,fft_len):
    F = np.fft.fft(x,fft_len,norm='ortho')
    F = np.abs(F)
    F = norm(F)
    Ff = (fs/2)*np.linspace(0,1,int(fft_len/2))
    Fdb = 20*np.log10(F[:int(len(F)/2)]);
    
    return F, Ff, Fdb

def norm( n ):
    return n/np.max(np.abs(n))

fs = 44100
sig_len = 8192 * 8 

bands = 5
step = (np.log(fs/2) - np.log(20)) / (bands-1)
cutoff = np.zeros(bands-1)
cutoff[0] = np.exp(step)*20

for i in range(1,bands-1):
    cutoff[i] = np.exp(step) * cutoff[i-1]

total_bands = bands - 1

gains = [1, 0.5, 1.2,1]

# input signal
h = signal.unit_impulse(sig_len)
sos = []
# first band lpf

for i in range(0, bands-2):
    sos_coeff = signal.butter(1,cutoff[i],'lp',fs=fs,output='sos')
    sos.append(sos_coeff)
    
    sos_coeff = signal.butter(1,cutoff[i],'hp',fs=fs,output='sos')
    sos.append(sos_coeff)

    print(cutoff[i])


y = []
num_filters = len(sos)

for i in range(num_filters):
    x = signal.sosfilt(sos[i],h)
    y.append(x)


y_sum = 0
Ydb_bands = []
Yf_bands = []

for i in range(num_filters):
    y_sum += y[i]
    Y,Yf,Ydb = fft(y[i],fs,sig_len)
    Ydb_bands.append(Ydb)
    Yf_bands.append(Yf)

Y,Yf,Ydb = fft(y_sum,fs,sig_len)

#plt.figure(1)
#plt.plot(y_sum)
plt.figure(2)
for i in range(num_filters):
    plt.semilogx(Yf_bands[i],Ydb_bands[i])
plt.semilogx(Yf,Ydb)
plt.hlines(-3,0,max(Yf))
plt.ylim(-30,5)
plt.xlabel('Frequency (Hz)')
plt.ylabel('Magnitude (dB)')
plt.show()
