import json
import random
import time
from math import log10
from datetime import datetime
import serial

from flask import Flask, Response, render_template

application = Flask(__name__)
random.seed()
ser = serial.Serial('COM3', 115200)
file = open('data_launch.txt', 'a')

pack_prev = 0
pack_now = 0
lost_pack = 0
pressure2 = -1
in_rocket = False
take_off = False
start_time = datetime.now()
vx = 0
vy = 0
vz = 0
sx, sy, sz = 0, 0, 0
SX, SY, SZ = 0, 0, 0
t_now, t_prev = 0, 0
trajectory = 0


def rec():
    arr = []
    global pressure2, in_rocket, take_off, trajectory, t_now, t_prev
    data = ser.readline()
    data = data.decode('utf-8')
    data = data.split()
    line = ""
    for i in data:
        arr.append(i)
        line += str(i) + " "
    print(line, file=file)
    if len(arr) == 8 and arr[1] == '2.00' and int(arr[6]) == 1:
        in_rocket = True
    if len(arr) == 8 and arr[1] == '1.00' and float(arr[4]) >= 20 and in_rocket == True:
        take_off = True
    if len(arr) == 8 and arr[1] == '0.00' and take_off == True:
        pressure2 = data[4]
        trajectory = 1
        t_now = float(arr[2]) / 1000
        t_prev = float(arr[2]) / 1000
    return arr


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


@application.route('/')
def plots():
    return render_template('index.html')


@application.route('/chart-data')
def chart_data():
    def generate_random_data():
        global pressure2, pack_prev, pack_now, lost_pack, vx, vy, vz, sx, sy, sz, trajectory, t_now, t_prev, SX, SY, SZ, in_rocket
        while True:
            #####################
            '''data = [0, 0, 0, 0, 0, 0, 0, 0]
            data[0] = pack_now
            data[2] = random.random() / 10
            data[3] = random.random() / 10
            data[4] = random.random() / 10
            data[5] = random.randint(0, 1)
            data[6] = round(random.random(), 5)
            data[7] = round(random.random(), 5)
            line = ""
            line += str(data[0]) + " " + str(data[1]) + " " + str(data[2]) + " " + str(data[3]) + " " + str(data[4])
            line += " " + str(data[5]) + " " + str(data[6]) + " " + str(data[7])
            print(line, file=file)
            cnt = ['0.00', '1.00', '2.00']
            n = random.randint(0, 2)
            data[1] = cnt[n]'''
            ########################################
            data = rec()
            #data = [0, 0, 0, 0, 0, 0, 0, 0]
            if len(data) != 8:
                continue
            pack_now = int(float(data[0]))

            if data[1] == '0.00':
                data[4] = float(data[4]) * 1000
                if pressure2 == -1:
                    delta_altitude = 0
                else:
                    if in_rocket:
                        data[5] = '1'
                    delta_altitude = 18400 * (1 + 0.003665 * data[3]) * log10(data[4] / pressure2)
                json_data = json.dumps(
                    {'type': 0, 'received':  zero(data[0] - lost_pack), 'lost':  zero(lost_pack), 'time': datetime.now().strftime('%H:%M:%S'), 'temperature': data[3],
                     'pressure': data[4], 'PH': data[5], 'COC': data[6], 'CAC': data[7], 'altitude': delta_altitude})
                pressure2 = data[4]
            elif data[1] == '1.00':
                t_now = data[2] / 1000
                delta = t_now - t_prev
                vx += float(data[3]) * delta
                vy += float(data[4]) * delta
                vz += float(data[5]) * delta
                sx = vx * delta + float(data[3]) * delta * delta / 2
                sy = vy * delta + float(data[4]) * delta * delta / 2
                sz = vz * delta + float(data[5]) * delta * delta / 2
                SX += sx
                SY += sy
                SZ += sz
                json_data = json.dumps(
                    {'type': 1, 'received': zero(data[0] - lost_pack), 'lost': zero(lost_pack), 'time': datetime.now().strftime('%H:%M:%S'), 'ax': data[3],
                     'ay': data[4], 'az': data[5], 'longitude': data[6], 'latitude': data[7], 'draw': trajectory, 'sx': SX, 'sy': SZ, 'sz': SY})
            elif data[1] == '2.00':
                json_data = json.dumps(
                    {'type': 2, 'received': zero(data[0] - lost_pack), 'lost': zero(lost_pack), 'time': datetime.now().strftime('%H:%M:%S'), 'roll': data[3],
                     'pitch': data[4], 'yaw': data[5]})

            lost_pack += pack_now - pack_prev
            pack_prev = pack_now
            t_prev = t_now
            yield f"data:{json_data}\n\n"
            time.sleep(0.1)

    return Response(generate_random_data(), mimetype='text/event-stream')

if __name__ == '__main__':
    application.run(debug=True, threaded=True)