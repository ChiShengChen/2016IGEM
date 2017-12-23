import DAN

ServerIP='140.113.199.248' #P=None:AutoSearch, or ='IP':Connect to this IP

def profile_init():
    DAN.profile['dm_name']='Pantide_board'
    DAN.profile['d_name']=DAN.get_mac_addr()[-4:]

def odf():  # int only
    return [
	('D6_LED', 0, 'D6_LED'),

    ]

def idf():
    return [
       ('A0_Soil', float),
       ('A1_Wind', float),
       ('A2_UV',   float),
       ('D3_Hum',  float),
       ('D3_Temp', float),
       ('D3_Pres', float),
	   ('D4_Rain', float),
	   ('D11_IR1', float),
	   ('D8_IR2',  float),
	   ('A3_IR3',  float),
    ]
