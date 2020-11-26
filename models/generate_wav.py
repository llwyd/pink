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
	return norm(pink)

len_mins = 45
fs = 44100
samples = fs * len_mins * 60

max_amplitude = np.iinfo(np.int16).max

pink_l = pink_filter( samples ) * max_amplitude
pink_r = pink_filter( samples ) * max_amplitude

pink_l = np.asarray(pink_l,dtype=np.int16)
pink_r = np.asarray(pink_r,dtype=np.int16)

pink = np.vstack((pink_l,pink_r))

wav.write('long_pink.wav',fs,pink_l)

