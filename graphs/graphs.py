import matplotlib.pyplot as plt #librería para graficas de python
import numpy # extiende las librerias de matemática de python
import pandas #libreria para procesamiento de datos muy usada en la industria
import seaborn # Extiende las funcionalidades de matplotlib, muy usado en data visualization

# Con esto hacemos los gráficos más grandes, adecuados para el proyector.
seaborn.set_context(context='paper', font_scale=1.2)

#CASO 1 Y 2 - los valores son los mismos, cambia cuello de botella
# Carga ofrecida vs carga recibida
off_ch_ps = [1, 1.425, 1.95, 3.295, 9.985]
rec_ch_ps = [1, 1.425, 1.945, 3.275, 4.99]
exp_rec_ch_ps = [1, 1.425, 1.95, 3.295, 9.985]
plt.plot(off_ch_ps, rec_ch_ps, color ='tab:pink', label="Real")
plt.plot(off_ch_ps, exp_rec_ch_ps, linestyle='--', color ='tab:purple', label='Esperada')
plt.title('Carga ofrecida vs carga recibida')
plt.xlabel('carga ofrecida (paq/seg)')
plt.ylabel('carga recibida (paq/seg)')
plt.legend()
plt.show()

#Carga ofrecida vs retardo (delay average)
av_delay= [0.424711, 0.431114, 0.456779, 0.575730, 32.879468835695]
plt.plot(off_ch_ps, av_delay, color ='tab:pink')
plt.title('Retardo en función de la carga ofrecida')
plt.xlabel('carga ofrecida (paq/seg)')
plt.ylabel('retardo (seg)')
plt.show()

# CASO 1
