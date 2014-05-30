#!/usr/bin/env python
# coding: utf-8

import pyjsonrpc

def add(a=None, b=None):
    """Test function"""
    return a + b

def handler(*args, **kwargs):
		return args, kwargs

methods_desc = {
		"bulk" : {
				"f" :  {
						"desc" : """Absolute path to a file which contains the list of ports to
build.  Ports must be specified in the form category/port
and shell-style comments are allowed.  Multiple -f file
arguments may be specified at once.""",
						},
				"a" : {
						"desc": "Build all ports in the tree",
						},
				"B" : {
						"desc" : """Specify which buildname to use.  By default
YYYY-MM-DD_HH:MM:SS will be used.  This can be used to
resume a previous build and use the same log and URL paths.""",
						},
				"c" : {
						"desc" : "Clean all previously built packages and logs.",
						},
				"C" : {
						"desc": "Clean only the packages specified in the file given in -f file",
						},
				"F" : {
						"desc" : "Only fetch from original MASTER_SITES.  Skip FreeBSD mirrors",
						},
				"j" : {
						"desc" : "Run the bulk build on the jail named name.",
						},
				"J" : {
						"desc" : """This argument specifies how many number jobs will run in
parallel for a bulk build.""",
						},
				"N" : {
						"desc" : "Do not build package repository or INDEX when build is completed",
						},
				"p" : {
						"desc" : "This flag specifies on which ports tree the bulk build will be done",
						},
				"s" : {
						"desc" : """Skip sanity tests.  Sanity tests are made to check if the
ports exists, does not have an increased version number, and
if the compiled options match the current options from the
make.conf files and /var/db/options.""",
						},
				"t" : {
						"desc" : """Add some testing. Currently uninstalls the port, and disable
parallel jobs for make.""",
						},
				"T" : {
						"desc" : """Try building BROKEN ports by defining TRYBROKEN for the build""",
						},
				"w" : {
						"desc" : """Save WRKDIR on build failure.  The WRKDIR will be tarred up
into ${POUDRIERE_DATA}/wrkdirs.""",
						},
				"v" : {
						"desc" : """This will show more information during the build.  Specify
twice to enable debug output.""",
						},
				"z" : {
						"desc" : """This specifies which SET to use for the build.  See
CUSTOMISATION for examples of how this is used.""",
						},
				},
		"jail": {
				"c" : {
						"desc" : "Creates a jail.",
						},
				"d" : {
						"desc" : "Deletes a jail.",
						},
				"l" : {
						"desc" : "List all available jails.",
						},
				"s" : {
						"desc" : "Starts a jail.",
						},
				"k" : {
						"desc" : "Kills a jail (stops it).",
						},
				"u" : {
						"desc" : "Update a jail.",
						},
				"J" : {
						"desc" : """This argument specifies how many number jobs will run in
parallel for a bulk build.""",
						},
				"q" : {
						"desc" : """"Remove the header when -l is the specified mandatory option.,
Otherwise, it has no effect.""",
				},
				"j" : {
						"desc" : "Run the bulk build on the jail named name.",
						},
				"v" : {
						"desc" : """Specifies which version of FreeBSD to use in the jail.  If
you are using method ftp then the version should in the form
of: 9.0-RELEASE.  If you are using method csup then the
version should be in the form of cvs branches: RELENG_9 or .
for current.  If you are using method svn then the version
should be in the form of svn branches: stable/9 or head for
current.""",
				},
				"a" : {
						"desc" : """Specifies which architecture of FreeBSD to use in the jail.
(Default: same as host)""",
				},
				"m" : {
						"desc" : """Specifies which method to use to create the jail.  Could be
csup, svn(,+http,+https,+file,+ssh), ftp, allbsd, or gjb
(Default: ftp).""",
				},
				"f" : {
						"desc" : "Specifies the filesystem name (${ZPOOL}/jails/filesystem).",
						},
				"M" : {
						"desc" : "Gives an alternative mountpoint when creating jail.",
						},
				"p" : {
						"desc" : "This specifies which port tree to start/stop the jail with.",
						},
				"t" : {
						"desc" : """Instead of upgrading to the latest security fix of the jail
version, you can jump to the new specified version.""",
						},
				"z" : {
						"desc" : """This specifies which SET to start/stop the jail with.""",
						},
				},
		"ports":{
				"c" :  {
						"desc" : "Creates a ports tree.",
						},
				"d" : {
						"desc" : "Deletes a ports tree.",
						},
				"u" : {
						"desc" : "Updates a ports tree.",
						},
				"l" : {
						"desc" : "List all available ports trees."
						},
				"q" : {
						"desc" : """Remove the header when -l is the specified subcommand.  Otherwise,
it has no effect.""",
						},
				"p" : {
						"desc" : "Specifies the name of the ports tree to use.",
						},
				"F" : {
						"desc" : """When used with -c, only create the needed ZFS file systems
and directories, but do not populate them.""",
						},
				"f" : {
						"desc" : "Specifies the filesystem name (${ZPOOL}/jails/filesystem).",
						},
				"M" : {
						"desc" : "Gives an alternative mountpoint when creating ports tree.",
						},
				"m" : {
						"desc" : """Specifies which method to use to create the ports tree.
Could be portsnap, git, svn{,+http,+https,+file,+ssh}
(Default: portsnap).""",
						},
				"v" : {
						"desc" : """Show more verbose output.""",
						},
				"B" : {
						"desc" : """Specifies which branch to checkout when using the svn
method.  (Default: head)""",
						},
				},
		"testport": {
				"o" : {
						"desc" : "Specifies an origin in the ports tree",
						},
				"c" : {
						"desc" : "Run make config for the given port.",
						},
				"j" : {
						"desc" : "Runs only inside the jail named name.",
						},
				"J" : {
						"desc" : """This argument specifies how many number jobs will run in
parallel for building the dependencies.""",
						},
				"n" : {
						"desc" : "Do not use custom prefix.",
						},
				"p" : {
						"desc" : "Specifies which ports tree to use.",
						},
				"s" : {
						"desc" : """Skip sanity tests.  Sanity tests are made to check if the,
ports exists, does not have an increased version number, and
if the compiled options match the current options from the
make.conf files and /var/db/options.""",
						},
				"v" : {
						"desc" : """This will show more information during the build.  Specify
twice to enable debug output.""",
						},
				"z" : {
						"desc" : """This specifies whith SET to use for the build.  See
CUSTOMISATION for examples of how this is used.""",
						},
				},
		"options":{
				"f" : {
						"desc" : """Absolute path to a file which contains the list of ports to
configure.  Ports must be specified in the form
category/port and shell-style comments are allowed.""",
						},
				"c" : {
						"desc" : """Use 'config' target, which will always show the dialog for
the given ports.""",
						},
				"C" : {
						"desc" : """Use 'config-conditional' target, which will only bring up
the dialog on new options for the given ports.  (This is the
default)""",
						},
				"j" : {
						"desc" : """If given, configure the options only for the given jail""",
						},
				"p" : {
						"desc" : """Run the configuration inside the given ports tree (by
default uses default)""",
						},
				"n" : {
						"desc" : """Do not be recursive."""
						},
				"r" : {
						"desc" : """Remove port options instead of configuring them""",
						},
				"s" : {
						"desc" : """Show port options instead of configuring them""",
						},
				"z" : {
						"desc" : """This specifies whith SET to use for the build.  See
CUSTOMISATION for examples of how this is used.""",
						},
				},
		"distclean":{
				"J" : {
						"desc" : """This argument specifies how many number jobs will run in
parallel for gathering distfile information.""",
						},
				"n" : {
						"desc" : """Dry run, do not actually delete anything.""",
						},
				"p" : {
						"desc" : """Specifies which ports tree to use.""",
						},
				"y" : {
						"desc" : """Assume yes, do not confirm and just delete the files.""",
						},
				"v" : {
						"desc" : """This will show more information during the build.  Specify
twice to enable debug output.""",
						},
},
		"queue":{},
		"status":{
				"j" : {
						"desc" : "Specifies the name of the jail to view the status of.",
						},
				"p" : {
						"desc" : """This flag specifies which ports tree the build is running on
when using -j""",
						},
				"z" : {
						"desc" : """This specifies which SET the build is running on when using -j""",
						},
				},
		"version":{},
		}



methods_dict = {}
for k in methods_desc.keys():
		methods_dict.update({k:handler})

print methods_dict
class RequestHandler(pyjsonrpc.HttpRequestHandler):

    # Register public JSON-RPC methods
    methods = methods_dict

# Threading HTTP-Server
http_server = pyjsonrpc.ThreadingHttpServer(
    server_address = ('localhost', 8080),
    RequestHandlerClass = RequestHandler
)
print "Starting HTTP server ..."
print "URL: http://localhost:8080"
http_server.serve_forever()
