#coding=utf-8

from base import BaseSB
import __init__ as libSB
from hashlib import md5 as __md5__

md5 = lambda x:__md5__(str(x)).hexdigest()
class SB1(BaseSB):
    '''
    �����ȫ��������Ϣ�ĺ���
    ���ڽ��ղ��֣�ÿ��������ͬ���Լ�ʹ��װ�������庯��.
    '''

    def login(self,username,password,clientversion=libSB.__version__,clientname=libSB.__name__):
        self.send(type=201,
            data='%s %s %s %s' % (username,
                                  md5(md5(username)+md5(password)),
                                  clientversion,
                                  clientname))

    def alive(self):
        self.send(type=20)

    def getuserlist(self):
        self.send(type=300)
        data = self.recv()
        print data
        return data[4].split(',')

    def sendmessage(self,to,message):
        self.send(type=100,
        data='%s %s' % (to,message))

    def logout(self):
        self.send(type=30)
    
