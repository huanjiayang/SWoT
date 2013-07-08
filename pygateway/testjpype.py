import jpype
jvmPath = jpype.getDefaultJVMPath()
jvmArg = "-DyourProperty=yourValue"
ext_classpath = "D:\Working space"
jvmArg ="-Djava.class.path =" +ext_classpath
if not jpype.isJVMStarted():
    jpype.startJVM(jvmPath)
jpype.shutdownJVM()