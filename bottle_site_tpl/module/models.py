#!/usr/bin/python
# -*- coding: utf-8 -*-

import datetime

from autumn.db.connection import autumn_db
from autumn.model import Model

from setting import DATABASE

autumn_db.conn.connect(
    DATABASE['engine'],
    db=DATABASE['db'],
    user=DATABASE['user'],
    passwd=DATABASE['passwd'],
    host=DATABASE['host'],
    port=DATABASE['port'],
    use_unicode=True
    )

class PDConf(Model):
    '''产品配置'''
    pass

class Account(Model):
    '''账户信息'''
    class Meta:
        defaults = {'status': 0, 'create_time': datetime.datetime.now()}

class UserInfo(Model):
    '''用户数据'''
    pass

class CardConf(Model):
    '''卡片配置'''
    pass
