#!/usr/bin/python
# -*- coding: utf-8 -*-

import setting

from bottle import Bottle
from bottle import debug, run
from bottle import redirect, abort, static_file
from bottle import jinja2_template as template
from bottle import request, response, local

from mysession import get_session_info, set_session_info
from mysession import deco_session_check

app = Bottle()

app.mount('/test', __import__('test').app)

@app.route('/center')
def handel_redirect():
    return redirect(request.path+'/')

@app.route('/')
@deco_session_check
def handle_index():
    return '我是首页'

if __name__ == '__main__':
    debug(True)
    run(app, host="0.0.0.0",reloader=True)
