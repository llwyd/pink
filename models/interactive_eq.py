import numpy as np
import matplotlib.pyplot as plt
from matplotlib.widgets import Slider, Button, RadioButtons

fig, ax = plt.subplots()
plt.subplots_adjust(bottom=0.35)
plt.xlabel('Frequency (Hz)')
plt.ylabel('Magnitude (dB)')

l, = plt.plot([1,2,3,4])

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

