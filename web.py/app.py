#!/usr/bin/env python
import web
import web.httpserver as httpserver
import web.wsgi as wsgi
from web.net import validaddr, validip
from web.httpserver import StaticMiddleware,LogMiddleware,WSGIServer
# from web.wsgiserver import CherryPyWSGIServer
from wsgiserver import CherryPyWSGIServer
from web.wsgiserver import _SHUTDOWNREQUEST
import sys, os
from web import utils,net

# redefine render method for web.form.Form class

def stderr_return(s):
		print >> sys.stderr, s

def radioRender(self):
		x = ""
		for arg in self.args:
				if isinstance(arg, (tuple, list)):
						value, desc, class_ = arg
				else:
						value, desc = arg, arg
				attrs = self.attrs.copy()
				attrs['name'] = self.name
				attrs['type'] = 'radio'
				attrs['value'] = value
				if self.value == value:
						attrs['checked'] = 'checked'
				x += '<div class="%s"><input %s/> %s</div>' % (class_, attrs, net.websafe(desc))
		return x
web.form.Radio.render = radioRender

def render(self):
		out = ''
		out += self.rendernote(self.note)
		out += '<div>'
		for i in self.inputs:
				html = i.render() + self.rendernote(i.note)
				out += '%s<div>%s\t%s</div>%s' % (utils.safeunicode(i.pre),
								net.websafe(i.description) ,
								html,
								utils.safeunicode(i.post)
								)
		out += "</div>\n"
		return out

web.form.Form.render = render

if __name__ == '__main__':
		optvars = [
						"PORT_OPTIONS",
						"NEW_OPTIONS",
						"ALL_OPTIONS",
						"OPTIONS_GROUP", # checkbox group
						"OPTIONS_MULTI", # checkbox group
						"OPTIONS_SINGLE", # radio box
						"OPTIONS_RADIO", # radio with empty choise box
						]
		optvalues = {}
		# in terms of HTML-forms OPTIONS_GROUP and OPTIONS_MULTI are the same
		type_associations = {
						"OPTIONS_GROUP" : web.form.Checkbox,
						"OPTIONS_MULTI" : web.form.Checkbox,
						"OPTIONS_SINGLE" : web.form.Radio,
						"OPTIONS_RADIO" : web.form.Radio,
						}
		for opt in "PORT_OPTIONS", "NEW_OPTIONS", "ALL_OPTIONS":
				o = os.getenv(opt)
				if o: o = o.split()
				locals().update({opt.lower():o})
		for opt in "OPTIONS_GROUP", "OPTIONS_MULTI", "OPTIONS_SINGLE", "OPTIONS_RADIO":
				g = os.getenv(opt)
				if g: g = g.split()
				else: g = []
				optvalues[opt] = {}
				for e in g:
						e_desc = os.getenv(e + "_DESC")
						if not e_desc: e_desc = ""
						optvalues[opt].setdefault(e,{"desc": e_desc , "dict": {}})
						g_opts = os.getenv(opt + "_" + e)
						if g_opts:
								for o in g_opts.split():
										#if opt in ("OPTIONS_GROUP","OPTIONS_MULTI"):
										checked = False
										new = "old"
										if o in port_options:
												checked = True
										if o in new_options:
												new = "new"
										optvalues[opt][e]["dict"].update({o: {"desc": os.getenv(o + '_DESC'),
												"checked": checked, "new":new}})
										#if opt in ("OPTIONS_SINGLE", "OPTIONS_RADIO"):
										#		checked = False
										#		new = "old"
										#		if o in port_options:
										#				checked = True
										#		if o in new_options:
										#				new = "new"
										#		optvalues[opt][e]["dict"].update({o: {"desc": os.getenv(o + '_DESC'),
										#				"checked": checked, "new":new, "type":type_associations[opt]}})
										#		print opt,e,o,os.getenv(o + '_DESC'),checked, new

		tmpdict = {}

		tmpdict.update(optvalues["OPTIONS_MULTI"])
		tmpdict.update(optvalues["OPTIONS_GROUP"])

		itemlist = []

		for k,v in tmpdict.items():
				local_itemlist = [
								web.form.Checkbox(
										v1["desc"],
										value=k1,
										checked=v1["checked"],
										class_=v1["new"]) for k1,v1 in v["dict"].items()
								]
				local_itemlist[0].pre = '<div id="%s" class="itemgroup">%s' % (k,v["desc"])
				local_itemlist[-1].post = '</div>'
				itemlist.extend(local_itemlist)
		tmpdict.update(optvalues["OPTIONS_MULTI"])

		for k,v in tmpdict.items():
				# k - group name
				# v["desc"] - group describtion
				# v["dict"] - group members
				itemlist.append(
								web.form.Radio(
										k,
										[ (k1,v1["desc"],v1["new"]) for k1,v1 in v["dict"].items() ],
										desc=v["desc"],
										)
								)
						
						# k1 - item name
						# v1["new"] - is item new
						# v1["checked"] - is item checked
						# v1["desc"] - item description

				#itemlist.extend(local_itemlist)

		itemlist.append(web.form.Button('Submit'))
		my_form = web.form.Form(*itemlist)

		urls = ('/', 'D4Web')
		render = web.template.render('templates/')

		app = web.application(urls, globals())

		func = app.wsgifunc()
		server_address = validip('127.0.0.1',8080)
		func = StaticMiddleware(func)
		#func = LogMiddleware(func)
		server = CherryPyWSGIServer(server_address, func,
						server_name="localhost",numthreads=1)
		#print "http://%s:%d/" % server_address
		class D4Web:
				def GET(self):
						form = my_form()
						return render.D4Web( form )
						form.validates()
						s = str(form.value)
						stderr_return(s)
						return s
		try:
				server.start()
		except (KeyboardInterrupt, SystemExit):
				server.stop()
				server = None
