import math


class Measurement(object):

    freq = 0
    refRe = 0
    refIm = 0
    measRe = 0
    measIm = 0

    def __init__(self, freq, refRe, refIm, measRe, measIm):
        self.freq = float(freq)
        self.refRe = float(refRe)
        self.refIm = float(refIm)
        self.measRe = float(measRe)
        self.measIm = float(measIm)

    def magnitude(self):
        return math.sqrt(self.measRe*self.measRe + self.measIm*self.measIm)\
               / math.sqrt(self.refRe*self.refRe + self.refIm*self.refIm)

    def angle(self):
        return math.atan2(self.measIm, self.measRe) - math.atan2(self.refIm, self.refRe)

    def print(self, file):
        file.write(','.join([str(self.freq), str(self.refRe), str(self.refIm), str(self.measRe), str(self.measIm),
                             str(self.magnitude()), str(self.angle())]))
