#####################################
#
#   - Дать Сене роверить соответствие осей цилинда и углов
#   - Дать Сене на проверку вычисление полученных/отерянных файлов
#
####################################

import json
import random
import time
from math import log10
from datetime import datetime
import serial

from flask import Flask, Response, render_template

application = Flask(__name__)
random.seed()
#ser = serial.Serial('COM5', 115200)
file = open('data_launch.txt', 'a')

pack_prev = -1
pack_now = 0
lost_pack = 0
pressure2 = 100000

start_time = datetime.now()
print(start_time)

'''def rec():
    data = ser.readline()
    data = data.decode('utf-8')
    data = data.split()
    return data'''


def zero(num):
    if num <= 9:
        return "0000" + str(num)
    elif num <= 99:
        return "000" + str(num)
    elif num <= 999:
        return "00" + str(num)
    elif num <= 9999:
        return "0" + str(num)
    else:
        return str(num)


@application.route('/plots')
def plots():
    return render_template('index.html')


@application.route('/chart-data')
def chart_data():
    def generate_random_data():
        global pressure2, pack_prev, pack_now, lost_pack
        while True:
            data = [0, 0, 0, 0, 0, 0, 0, 0]
            data[0] = pack_now
            data[2] = random.random()
            data[3] = random.random()
            data[4] = random.random()
            data[5] = random.randint(0, 1)
            data[6] = round(random.random(), 5)
            data[7] = round(random.random(), 5)
            line = ""
            line += str(data[0]) + " " + str(data[1]) + " " + str(data[2]) + " " + str(data[3]) + " " + str(data[4])
            line += " " + str(data[5]) + " " + str(data[6]) + " " + str(data[7])
            print(line, file=file)
            cnt = ['0.00', '1.00', '2.00']
            n = random.randint(0, 2)
            data[1] = cnt[n]
            if data[1] == '0.00':
                delta_altitude = 18400 * (1 + 0.003665 * data[3]) * log10(data[4] / pressure2)
                json_data = json.dumps(
                    {'type': 0, 'received':  zero(data[0] - lost_pack), 'lost':  zero(lost_pack), 'time': datetime.now().strftime('%H:%M:%S'), 'temperature': data[3],
                     'pressure': data[4], 'PH': data[5], 'COC': data[6], 'CAC': data[7], 'altitude': delta_altitude})
                pressure2 = data[4]
            elif data[1] == '1.00':
                json_data = json.dumps(
                    {'type': 1, 'received': zero(data[0] - lost_pack), 'lost': zero(lost_pack), 'time': datetime.now().strftime('%H:%M:%S'), 'ax': data[3],
                     'ay': data[4], 'az': data[5], 'longitude': data[6], 'latitude': data[7]})
            elif data[1] == '2.00':
                pack_now += 1                           #убрать
                json_data = json.dumps(
                    {'type': 2, 'received': zero(data[0] - lost_pack), 'lost': zero(lost_pack), 'time': datetime.now().strftime('%H:%M:%S'), 'roll': data[3],
                     'pitch': data[4], 'yaw': data[5]})

            lost_pack += pack_now - pack_prev - 1
            pack_prev = pack_now
            pack_now += 1                               #убрать
            yield f"data:{json_data}\n\n"
            time.sleep(0.1)

    return Response(generate_random_data(), mimetype='text/event-stream')

if __name__ == '__main__':
    application.run(debug=True, threaded=True)
