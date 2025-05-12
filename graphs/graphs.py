import matplotlib.pyplot as plt #librería para graficas de python
import numpy # extiende las librerias de matemática de python
import pandas #libreria para procesamiento de datos muy usada en la industria
import seaborn # Extiende las funcionalidades de matplotlib, muy usado en data visualization

# Con esto hacemos los gráficos más grandes, adecuados para el proyector.
seaborn.set_context(context='paper', font_scale=1.2)

# #CASO 1 Y 2 - los valores son los mismos, cambia cuello de botella
# # Carga ofrecida vs carga recibida
# off_ch_ps = [1, 1.425, 1.95, 3.295, 9.985]
# rec_ch_ps = [1, 1.425, 1.945, 3.275, 4.99]
# exp_rec_ch_ps = [1, 1.425, 1.95, 3.295, 9.985]
# plt.plot(off_ch_ps, rec_ch_ps, color ='tab:pink', label="Real")
# plt.plot(off_ch_ps, exp_rec_ch_ps, linestyle='--', color ='tab:purple', label='Esperada')
# plt.title('Carga ofrecida vs carga recibida')
# plt.xlabel('carga ofrecida (paq/seg)')
# plt.ylabel('carga recibida (paq/seg)')
# plt.legend()
# plt.show()

# #Carga ofrecida vs retardo (delay average)
# av_delay= [0.424711, 0.431114, 0.456779, 0.575730, 32.879468835695]
# plt.plot(off_ch_ps, av_delay, color ='tab:pink')
# plt.title('Retardo en función de la carga ofrecida')
# plt.xlabel('carga ofrecida (paq/seg)')
# plt.ylabel('retardo (seg)')
# plt.show()

# CASO 1 - chequeo de buffers y quién droppea paquetes
usecols = ["vectime", "vecvalue"]
data_c1 = pandas.read_csv('c1-PackDrop-0-1.csv', usecols=usecols)
# imprimo lo leido para saber cómo indexar las filas
file = open("data_c1.txt", "w")
file.write(str(data_c1))
file.close()

# tiempo vs tamaño de buffers
# extraigo los datos del archivo y los convierto en una lista
time_gen = list(map(float, (data_c1['vectime'].iloc[26]).split()))
time_queue = list(map(float, (data_c1['vectime'].iloc[28]).split()))
time_sink = list(map(float, (data_c1['vectime'].iloc[30]).split()))
buffer_gen = list(map(float, (data_c1['vecvalue'].iloc[26]).split()))
buffer_queue = list(map(float, (data_c1['vecvalue'].iloc[28]).split()))
buffer_sink = list(map(float, (data_c1['vecvalue'].iloc[30]).split()))

plt.plot(time_gen, buffer_gen, color='tab:cyan', label="Nodo transmisor")
plt.plot(time_queue, buffer_queue, color='tab:pink', label="Cola intermedia, la red")
plt.plot(time_sink, buffer_sink, color='tab:purple', label="Nodo receptor")
plt.title('Tamaño de los buffers de la red')
plt.xlabel('tiempo de simulación')
plt.ylabel('cantidad de paquetes en en el buffer')
plt.legend()
plt.show()

# tiempo vs paquetes perdidos
# extraigo y enlisto
dropped_time_sink = list(map(float, (data_c1['vectime'].iloc[33]).split()))
dropped_pck_sink = list(map(float, (data_c1['vecvalue'].iloc[33]).split()))

plt.plot(dropped_time_sink, dropped_pck_sink, color='tab:pink')
plt.title('Pérdida de paquetes en el nodo receptor')
plt.xlabel('tiempo de simulación')
plt.ylabel('paquetes perdidos hasta el momento')
plt.show()

# CASO 2 - mismos gráficos que para el caso 1, pero ahora revisamos la queue intermedia

usecols = ["vectime", "vecvalue"]
data_c2 = pandas.read_csv('c2-PackDrop-0-1.csv', usecols=usecols)
# imprimo lo leido para saber cómo indexar las filas
file = open("data_c2.txt", "w")
file.write(str(data_c2))
file.close()

# tiempo vs tamaño de buffers
# extraigo los datos del archivo y los convierto en una lista
time_gen = list(map(float, (data_c2['vectime'].iloc[26]).split()))
time_queue = list(map(float, (data_c2['vectime'].iloc[28]).split()))
time_sink = list(map(float, (data_c2['vectime'].iloc[30]).split()))
buffer_gen = list(map(float, (data_c2['vecvalue'].iloc[26]).split()))
buffer_queue = list(map(float, (data_c2['vecvalue'].iloc[28]).split()))
buffer_sink = list(map(float, (data_c2['vecvalue'].iloc[30]).split()))

plt.plot(time_gen, buffer_gen, color='tab:cyan', label="Nodo transmisor")
plt.plot(time_queue, buffer_queue, color='tab:pink', label="Cola intermedia, la red")
plt.plot(time_sink, buffer_sink, color='tab:purple', label="Nodo receptor")
plt.title('Tamaño de los buffers de la red')
plt.xlabel('tiempo de simulación')
plt.ylabel('cantidad de paquetes en en el buffer')
plt.legend()
plt.show()

# tiempo vs paquetes perdidos
# extraigo y enlisto
dropped_time_queue = list(map(float, (data_c2['vectime'].iloc[33]).split()))
dropped_pck_queue = list(map(float, (data_c2['vecvalue'].iloc[33]).split()))

plt.plot(dropped_time_queue, dropped_pck_queue, color='tab:pink')
plt.title('Pérdida de paquetes en la cola intermedia')
plt.xlabel('tiempo de simulación')
plt.ylabel('paquetes perdidos hasta el momento')
plt.show()
