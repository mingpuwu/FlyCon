import os
import socket

class Pid:
    def __init__(self,kp,ki,kd) -> None:
        self.Kp = kp
        self.Ki = ki
        self.Kd = kd

        # /* Derivative low-pass filter time constant */
        self.tau = 0.02

        # /* Output limits */
        self.limMin = -10.0
        self.limMax = 10.0

        # /* Integrator limits */
        self.limMinInt = -5.0
        self.limMaxInt = 5.0

        # /* Sample time (in seconds) */
        self.T = 0.01

        # /* Controller "memory" */
        self.integrator = 0.0
        self.prevError = 0.0
        self.differentiator = 0.0
        self.prevMeasurement = 0.0

        # /* Controller output */
        self.out = 0.0

    def Update(self, setpoint, measurement):
        # /*
        #  * Error signal
        #  */
        error = setpoint - measurement

        # /*
        #  * Proportional
        #  */
        proportional = self.Kp * error

        # /*
        #  * Integral
        #  */
        self.integrator = self.integrator + 0.5 * self.Ki * self.T * (error + self.prevError)

        # /* Anti-wind-up via integrator clamping */
        if (self.integrator > self.limMaxInt):
            self.integrator = self.limMaxInt
        elif (self.integrator < self.limMinInt):
            self.integrator = self.limMinInt

        # /*
        #  * Derivative (band-limited differentiator)
        #  */

        self.differentiator = -(2.0 * self.Kd * (measurement - self.prevMeasurement)
                                + (2.0 * self.tau - self.T) * self.differentiator) / (2.0 * self.tau + self.T)

        # /*
        #  * Compute output and apply limits
        #  */
        self.out = proportional + self.integrator + self.differentiator

        if (self.out > self.limMax):
            self.out = self.limMax
        elif (self.out < self.limMin):
            self.out = self.limMin

        # /* Store error and measurement for later use */
        self.prevError = error
        self.prevMeasurement = measurement

        # /* Return controller output */
        return self.out
    
global_output = 0.0
global_alpha = 0.02

def TestSystem_Update(inp):
    global global_output
    global global_alpha

    global_output = (0.002 * inp + global_output) / (1.0 + global_alpha * 0.01)

    return global_output

if __name__ == '__main__':
    pid  = Pid(100,30,3)
    client = socket.socket()
    server_ip_port = ('192.168.1.9',1347)
    client.connect(server_ip_port)
    t = 0.0
    while(t < 10.0):
        # /* Get measurement from system */
        # measurement = TestSystem_Update(pid.out)
        measurement = 5

        # /* Compute new control signal */
        pid.Update(5.0, measurement)

        print(measurement, pid.out)
        client.send((str(measurement)+','+str(pid.out)+'\r\n').encode())
        t = t + 0.01
    client.close()

    