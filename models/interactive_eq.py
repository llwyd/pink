import numpy as np
import matplotlib.pyplot as plt
from matplotlib.widgets import Slider, Button, RadioButtons
from scipy import signal

class EQBand():
    def __init__(self,lp_cutoff,hp_cutoff,fs):
        order = 1
        self.fs = fs
        self.gain = 1.0
        if hp_cutoff == 0:
            # low pass only
            self.high = [1,0,0,1,0,0]
        else:
            self.high = signal.butter(1,hp_cutoff,'hp',fs=fs,output='sos')

        if lp_cutoff == 0:
            # high pass only
            self.low = [1,0,0,1,0,0]
        else:
            self.low = signal.butter(1,lp_cutoff,'lp',fs=fs,output='sos')
            
def fft(x,fs,fft_len):
    F = np.fft.fft(x,fft_len,norm='ortho')
    F = np.abs(F)
    F = norm(F)
    Ff = (fs/2)*np.linspace(0,1,int(fft_len/2))
    Fdb = 20*np.log10(F[:int(len(F)/2)]);
    
    return F, Ff, Fdb

def norm( n ):
    return n/np.max(np.abs(n))

fig, ax = plt.subplots()
plt.subplots_adjust(bottom=0.35)
plt.xlabel('Frequency (Hz)')
plt.ylabel('Magnitude (dB)')

fs = 44100
band = EQBand(500,1000,fs)
sig_len = 8192 * 8 
h = signal.unit_impulse(sig_len)
x = signal.sosfilt(band.low,h)
x = signal.sosfilt(band.high,x)

X,Xf,Xdb = fft(x,fs,sig_len)

l, = plt.semilogx(Xf,Xdb)
plt.hlines(-3,0,max(Xf))
plt.ylim(-30,5)
plt.xlabel('Frequency (Hz)')
plt.ylabel('Magnitude (dB)')

def update_graph(val):
    print(band_0.val)

axcolor = 'lightgoldenrodyellow'
axfreq_0 = plt.axes([0.2, 0.225, 0.65, 0.03], facecolor=axcolor)
axfreq_1 = plt.axes([0.2, 0.175, 0.65, 0.03], facecolor=axcolor)
axfreq_2 = plt.axes([0.2, 0.125, 0.65, 0.03], facecolor=axcolor)
axfreq_3 = plt.axes([0.2, 0.075, 0.65, 0.03], facecolor=axcolor)
axfreq_4 = plt.axes([0.2, 0.025, 0.65, 0.03], facecolor=axcolor)

band_0 = Slider(axfreq_0, 'Band 0', 0.01, 100, valinit=1, valstep=0.5)
band_1 = Slider(axfreq_1, 'Band 1', 0.01, 100, valinit=1, valstep=0.5)
band_2 = Slider(axfreq_2, 'Band 2', 0.01, 100, valinit=1, valstep=0.5)
band_3 = Slider(axfreq_3, 'Band 3', 0.01, 100, valinit=1, valstep=0.5)
band_4 = Slider(axfreq_4, 'Band 4', 0.01, 100, valinit=1, valstep=0.5)

band_0.on_changed(update_graph)
band_1.on_changed(update_graph)
band_2.on_changed(update_graph)
band_3.on_changed(update_graph)
band_4.on_changed(update_graph)

plt.show()

