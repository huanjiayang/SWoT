import jpype
jvmPath = jpype.getDefaultJVMPath()
ext_classpath = "D:\Working space\MyEclipse\SWE"
jvmArg ="-Djava.class.path =" +ext_classpath
print jvmArg
if not jpype.isJVMStarted():
    jpype.startJVM(jvmPath)

javaClass = jpype.JClass('SWE.javaclass.JavaClass')
vlaue = "oldvalue"
javaInstance = javaClass(value)
print javaInstance.getValue()
javaInstance.setValue("newvalue")
print javaInstance.getValue()

jpype.shutdownJVM()
