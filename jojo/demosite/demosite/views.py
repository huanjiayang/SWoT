from django.http import HttpResponse
import datetime

def demotest(request):
    return HttpResponse("Hello! This is a test page!")

def currenttime(request):
    return HttpResponse("The current time is " + str(datetime.datetime.now()))



from django.template import Template, Context

def indexview(request):
    now = datetime.datetime.now()

    myfile = open('./templates/test.html')
    mytemplate = Template(myfile.read())
    myfile.close()
    html = mytemplate.render(Context({'your_name' : 'jojo','current_time': now}))
    return HttpResponse(html)