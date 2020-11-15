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

# input signal
h = signal.unit_impulse(sig_len)

# 1st order low pass butter
sos0 = signal.butter( 1, 1000, 'lp',fs=44100,output='sos')

# 1st order high pass butter
sos1 = signal.butter(1, 1000, 'hp',fs =44100,output='sos')

y0 = signal.sosfilt(sos0,h)
#y0 = signal.sosfilt(sos0,y0)

y1 = signal.sosfilt(sos1,h)
#y1 = signal.sosfilt(sos1,y1)

# add HPF and LPF components
y = y0 + y1


Y0,Y0f,Y0db = fft(y0,fs,sig_len)
Y1,Y1f,Y1db = fft(y1,fs,sig_len)
Y,Yf,Ydb = fft(y,fs,sig_len)


plt.semilogx(Y0f,Y0db)
plt.semilogx(Y1f,Y1db)
plt.semilogx(Yf,Ydb)
plt.hlines(-3,0,max(Yf))
plt.hlines(-6,0,max(Yf))
plt.ylim(-50,10)
plt.show()
