import time  # 导入时间模块，时间模块是Python自带模块，无需安装
import serial  # 导入串口模块
import serial.tools.list_ports

port_list = list(serial.tools.list_ports.comports())       #查找可用串口      

if len(port_list) == 0:                             
    print('无可用串口')
else:       
    print('可用串口为')
    for i in range(0,len(port_list)):                      #将串口号打印             
        print(port_list[i])

port_num = input("输入串口号:")                         
baud = input("输入波特率:") 

flag = 1                                                    
while(flag):
    try:
        #打开串口，并且获得串口对象
        SCI1 = serial.Serial(port_num,baud,timeout=50)
        #判断是否打开成功
        if (SCI1.isOpen()==True):
            print("串口已经打开！\n")
            flag = 0

    except Exception as exc:
        print("串口打开异常:",exc)
        port_num = input("输入串口号:")                     #重新输入串口号
        baud = input("输入波特率:")                         #重新输入波特率
        flag = 1

flag1 = 0 
while True: 
    command = input("请输入控制命令，s-向单片机发送数据；r-从单片机中读取数据；rf-收文件；sf-发文件；t-读系统时间; gt-滚动系统时间")
    print("命令为 ",command)


    if command == 'sf':
        file_name=input("发送文件名为")                      #输入文件名   
        file_name=file_name.replace('\\','\\\\')            #文件名格式转换
        with open (file_name,'r') as ft:  #读取文件
            contents=ft.read()                                               
            print(contents)                                 #打印文件数据
        SCI1.write(str(contents).encode("utf-8"))           #串口发送数据


    if command == 'r':
        SCI1.reset_input_buffer()                           #清空串口缓存
        print("接收数据为") 
        buffer = SCI1.readline()       #串口接收数据       以/n结尾
        buffer = str(buffer,'utf-8')   #数据处理
        print(buffer)                  #数据打印


    if command == 's':
        a = input("发送数据为")                 #输入发送数据
        SCI1.write((str(a)+'\n').encode("utf-8"))     #串口发送数据


    if command == 'rf':
        SCI1.reset_input_buffer()               #清空串口缓存      
        print("接收数据为") 
        buffer = SCI1.readline()                #接收数据
        b = str(buffer,'utf-8')                 #转换格式
        print(b)                                #打印输出

        file_name=input("接收文件名为")             #输入文件名
        file_name=file_name.replace('\\','\\\\')    #文件名格式转换
        with open(file_name, 'w+') as f:            #生成文件
            f.write(b)                              #写入文件                         


    if command == 't':
        with open ('C:\\Users\\Lenovo\\Desktop\\111\\test1.txt', 'w+') as file1: #打开文件，在文件末尾，追加数据
            file1.write("Now Time"+" ")
            file1.write(str(time.localtime().tm_year)+"/") #保存当前时间：年
            file1.write(str(time.localtime().tm_mon).zfill(2)+"/") #保存当前时间：月
            file1.write(str(time.localtime().tm_mday).zfill(2)+"/") #保存当前时间：日
            file1.write(str(time.localtime().tm_hour).zfill(2)+":") #保存当前时间：小时
            file1.write(str(time.localtime().tm_min).zfill(2)+":") #保存当前时间：分
            file1.write(str(time.localtime().tm_sec).zfill(2)+'\n')#保存当前时间：秒
        with open ('C:\\Users\\Lenovo\\Desktop\\111\\test1.txt','r') as ft:  #读取文件
            contents=ft.read()                                               #获取数据
            print(contents) 
            
        SCI1.write(str(contents).encode("utf-8"))                            #串口发送数据
    

    if command == 'gt':
        while True:
            if flag1 == 0:
                contents = str(time.localtime().tm_year)+"/"+str(time.localtime().tm_mon).zfill(2)+'\n'    #年月
                SCI1.write(str(contents).encode("utf-8"))                            #串口发送数据

            if flag1 == 1:
                contents= str(time.localtime().tm_mday)+"/"+str(time.localtime().tm_hour).zfill(2)+'\n'     #日时
                SCI1.write(str(contents).encode("utf-8"))                            #串口发送数据

            if flag1 == 2:
                contents= str(time.localtime().tm_min)+":"+str(time.localtime().tm_sec).zfill(2)+'\n'       #分秒
                SCI1.write(str(contents).encode("utf-8"))                            #串口发送数据
                flag1 = 0
                break

            time.sleep(2)
            flag1 = flag1 + 1
            if flag1 == 3:
                flag1 = 0

    if(input("如果退出，请输入n，如果继续发送命令，请按其他键 ") == 'n'):
        break

SCI1.close()    #关闭端口         
