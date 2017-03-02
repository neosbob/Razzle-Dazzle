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

def proxyTest():
    print "Proxy integration test:"
    subprocess.call(["make"])

    proxyProcess = Process(target=proxyThreadExec)
    proxyProcess.start()
    print "Hey!!!!!"
    sleep(0.01)

    req_server = requests.get('http://my.ucla.edu')
    req_proxy = requests.get('http://localhost:3001/')
    

    if req_server.status_code != 200 or req_proxy.status_code != 200:
	    print "Error: Wrong Status Code."
    else:
	    print "Status Code Correct: 200"

    
    if req_server.content == req_proxy.content:
        print "Proxy test passed!"
    else:
        print "Proxy test failed!"

    print "Shutting down web server"
    subprocess.call(["fuser", "-k", "3001/tcp"])
    subprocess.call(["make", "clean"])

    proxyProcess.terminate()

def main():
    proxyTest()
    
if __name__ == "__main__":
    main()
