import datetime as dt
import matplotlib.pyplot as plt
import numpy as np
import random as r
from scipy import signal
import scipy.io.wavfile as wav

def norm( n ):
	return n/np.max(np.abs(n));

def pink_filter( n ):
	# Taken from http://www.firstpr.com.au/dsp/pink-noise/
	# Filter design by Robert Bristow-Johnson
	p = [0.99572754,0.94790649,0.53567505] 
	z = [0.98443604,0.83392334,0.07568359] 
	k = 1

	# This converts to poles and zeros into second order sections
	sos = signal.zpk2sos( z, p, k );
	
	# Generate whitenoise
	# http://peabody.sapp.org/class/dmp2/lab/whitenoise/
	white = np.zeros(n);
	for i in range(len(white)):
		white[i] = (r.random()*2)-1
	
	# Perform filtering
	pink = signal.sosfilt(sos,white)
	return pink

#alsa version
a = np.loadtxt("pink.txt")

samples = len(a)
fft_len = int(samples/2)
pink = pink_filter( samples )
fs = 48000

# get FFT
pink=norm(pink)
P = np.fft.fft(pink,samples,norm='ortho');
P = np.abs(P);
Pf = (fs/2)*np.linspace(0,1,fft_len)
Pdb = 20*np.log10(P[:int(len(P)/2)]);

a=norm(a)
A = np.fft.fft(a,samples,norm='ortho');
A = np.abs(A);
Af = (fs/2)*np.linspace(0,1,fft_len)
Adb = 20*np.log10(A[:int(len(A)/2)]);


plt.figure(1)
plt.semilogx(Pf,Pdb)
plt.semilogx(Af,Adb)
plt.show()