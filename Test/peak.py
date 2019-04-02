import numpy as np
import matplotlib 

import matplotlib.pyplot as plt

def min_max(x, axis=None):
    min = x.min(axis=axis, keepdims=True)
    max = x.max(axis=axis, keepdims=True)
    result = (x-min)/(max-min)
    return result

DataArr =np.array([0,35.479937,98.733408,152.868914,189.989319,239.279718,297.688082,333.015413,326.987642,327.598047,338.737978,316.725161,238.59301,161.452768,125.476877,117.388979,87.173815,20.372347,-10.720274,42.041811,149.473524,268.617426,340.225847,343.239733,288.989779,202.197467,139.249201,125.667635,108.881426,82.328707,94.880205,127.765909,136.197161,118.609797,100.679079,106.630549,155.882803,240.386084,314.588739,344.498705,346.635134,338.470934,331.680147,325.385317,303.601404,258.354952,200.862201,144.895468,89.653597,41.584002,14.611635,27.84984,86.56341,175.186937,289.485735,405.119799,425.301394,380.779802,329.23852,286.700747,239.851976,184.495651,128.452616,84.465128,31.283388,-39.447584,-97.512587,-98.61894,-48.565535,12.17,76.949495,137.608728,191.057531,237.7537,280.863726,293.911188,291.774759,287.501909,297.077677,287.005953,266.824354,238.631161,208.530444,179.307188,150.732489,95.795817,43.110033,31.321535,37.349306,28.536547,25.179308,70.921709,153.059668,237.677401,308.522814,326.033876,295.589808,249.542196,209.331604,200.213643,192.888753,145.544027,89.653597,52.647646,36.509996,30.825579,20.3342,49.252245,131.008699,211.315428,254.997709,279.261408,288.989779,259.499464,247.634672,255.188463,241.263544,195.368533,156.378759,129.101175,115.786661,103.27331,103.158861,115.51961,116.015566,99.153063,104.913782,140.164812,219.212574,362.925371,475.965197,470.242632,416.831987,355.257128,275.25561,248.550284,264.72608,217.648405,107.965815,25.064852,5.41736,34.98398,109.186632,210.094613,297.840685,361.323061,386.082705,361.017855,307.034945,248.702883,194.719976,181.062109,212.154739,217.495804,160.003054,74.927507,28.30765,17.854418,61.841906,161.185717,263.619715,314.665041,290.515795,211.048375,171.753397,197.657561,198.382421,159.964903,106.058291,91.446665,134.938196,213.833359,273.233637,280.787424,246.413855,209.179002,171.142988,119.487261,52.762087,13.238207,24.721499,42.385164,39.752779,66.687014,146.268885,248.245077,302.380587,311.193346,311.307795,319.319399,305.470774,299.2904,274.225542,207.805585,161.872423,170.761484,158.744085,104.684877,51.159777,15.489092,-15.603544,-33.190901,-15.183882,30.367769,92.858233,169.616968,238.898214,287.349306,331.222338,358.347323,361.056002,367.198229,375.286135,350.335726,283.61056,210.132764,131.695406,49.557451,-9.499464,-25.522657,-26.094923,-29.871809,-19.304136,15.069429,50.396761,78.513659,125.095377,195.750038,272.241724,324.927515,372.615603,412.597277,425.453997,397.146349,358.271029,338.127581,317.106669,270.028994,202.006714,108.461771,41.240648,20.601244,25.179308,43.644137,78.818865,125.934687,176.941858,238.51671,315.275446,347.054781,336.639711,337.784227,354.532274,333.091715,290.7447,272.089121])

DataArr_n = min_max(DataArr)

resultArr = np.zeros(256)
xaxis = np.array(range(256))

before = 0
before_i = 0

try:
    for i ,data in enumerate(DataArr_n):
        data1 = DataArr_n[i]
        data2 = DataArr_n[i+1]
        data3 = DataArr_n[i+2]

        if data > 0.7 and  data1-data2 < 0 and data2 - data3 > 0 and i - before_i > 10:
            plt.plot(i+3,data2,marker='.')
            before_i = i
            
except:
    _ = 0
plt.plot(xaxis,DataArr_n,c = 'g',linewidth = '0.5')


plt.show()