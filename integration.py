import subprocess
from multiprocessing import Process
import urllib2
from time import sleep
import requests

SERVER_CONFIG_NAME = "config_file"
PROXY_CONFIG_NAME = "config_file_proxy"

def threadExec():
	subprocess.call(["./webserver", SERVER_CONFIG_NAME])

def proxyThreadExec():
    subprocess.call(["./webserver", PROXY_CONFIG_NAME])

def serverTest():
    print "Server integration test:"
    print "Building binary"
    subprocess.call(["make"])

    serverProcess = Process(target=threadExec)
    serverProcess.start()

    sleep(5)

    req = requests.get('http://localhost:3000')

    if req.status_code != 200:
	    print "Error: Wrong Status Code."
	    exit(1)
    else:
	    print "Status Code Correct: 200"

    if req.headers['content-type'] != 'text/plain':
	    print "Incorrect content-type"
	    exit(2)
    else:
	    print "Content Type Correct: text/plain"

    if req.headers['content-length'] != str(len(req.content)):
	    print "Error: Length Header Invalid"
	    exit(3)
    else: 
	    print "Content Length Correct"

    print "Integration Test Passed!"

    print "Shutting down web server"
    subprocess.call(["fuser", "-k", "3000/tcp"])

    print "Cleaning previous build"
    subprocess.call(["make", "clean"])

    serverProcess.terminate()

def proxyTest():
    print "Proxy integration test:"
    subprocess.call(["make"])

    serverProcess = Process(target=threadExec)
    serverProcess.start()
    proxyProcess = Process(target=proxyThreadExec)
    proxyProcess.start()

    sleep(5)

    req_server = requests.get('http://localhost:3000')
    req_proxy = requests.get('http://localhost:3001')
    

    if req_server.status_code != 200 or req_proxy.status_code != 200:
	    print "Error: Wrong Status Code."
    else:
	    print "Status Code Correct: 200"

    print "\nContent of request to server is:"
    print req_server.content
    
    print "Content of request to proxy is:"
    print req_proxy.content

    print "Shutting down web server"
    subprocess.call(["fuser", "-k", "3000/tcp"])
    subprocess.call(["fuser", "-k", "3001/tcp"])
    subprocess.call(["make", "clean"])

    serverProcess.terminate()
    proxyProcess.terminate()

def main():
    serverTest()
    proxyTest()
    
if __name__ == "__main__":
    main()
