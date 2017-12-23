import DAN

ServerIP='140.113.199.195' #P=None:AutoSearch, or ='IP':Connect to this IP

def profile_init():
    DAN.profile['dm_name']='MCU_board'
    DAN.profile['d_name']=DAN.get_mac_addr()[-4:]

def odf():  # int only
    return [
	('D2', 0, 'D2'),
	('D3', 0, 'D3'),
	('D4', 0, 'D4'),
	('D5~PWM', 0, 'D5~PWM'),
	('D6~PWM', 0, 'D6~PWM'),
        ('D7', 0, 'D7'),
        ('D8', 0, 'D8'),
        ('D9~PWM', 0, 'D9~PWM'),
    ]

def idf():
    return [
       ('A0', int),
       ('A1', int),
       ('A2', int),
       ('A3', int),
       ('A4', int),
       ('A5', int),
    ]
