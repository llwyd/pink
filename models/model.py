import datetime as dt
import matplotlib.pyplot as plt
import numpy as np
import random as r
from scipy import signal

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

def voss_algorithm( bands, n ):
	pink = np.zeros(n)
	noise = np.zeros(bands)
	
	for i in range(n):
		pink[i] = 0
		for j in range(bands):
			if((i+1)%np.power(2,j)==0):
				noise[j]=(r.random()*2)-1;
		for j in range(bands):
			pink[i]=pink[i]+noise[j];
		
		# scale
		pink[i]=pink[i]/2;
	return pink

samples = 4096
pink = pink_filter( samples )
voss = voss_algorithm(16, samples)
fs = 44100

# get FFT
P = np.fft.fft(pink,4096,norm='ortho');
P = np.abs(P);
Pf = (fs/2)*np.linspace(0,1,2048)
Pdb = 20*np.log10(P[:int(len(P)/2)]);


V = np.fft.fft(voss,4096,norm='ortho');
V = np.abs(V);
Vf = (fs/2)*np.linspace(0,1,2048)
Vdb = 20*np.log10(V[:int(len(V)/2)]);


plt.figure(1);
plt.subplot(2,1,1);
plt.plot(pink);
plt.plot(voss);
plt.xlabel("Samples");
plt.ylabel("Amplitude");
plt.subplot(2,1,2);
plt.semilogx(Pf,Pdb);
plt.semilogx(Vf,Vdb);
plt.ylabel("Magnitude");
plt.xlabel("Frequency (Hz)");
plt.show();

