INCLUDES = \
	-I$(top_srcdir)\
	-I$(top_builddir)\
	-I$(top_srcdir) \
	-I$(top_builddir) \
	$(PACKAGE_OPTIMASTER_CFLAGS) \
	-DDATADIR=\""$(datadir)"\" \
	-DLIBDIR=\""$(libdir)"\" \
	-DCONFDIR=\""$(sysconfdir)"\" \
	-DPREFIXDIR=\""$(prefix)"\" \
	-DPKGDATADIR=\""$(pkgdatadir)"\" \
	-DPKGLIBDIR=\""$(pkglibdir)"\"

bin_PROGRAMS = optimaster
optimaster_CFLAGS = D_GNU_SOURCE -g $(PACKAGE_OPTIMASTER_CFLAGS)
optimaster_LDFLAGS = $(PACKAGE_OPTIMASTER_LIBS)
optimaster_SOURCES = \
	optimaster.cc \
	Application/run.cc \
	Application/onworkerresponse.cc \
	Application/application.cc \
	Application/registerworker.cc \
	Application/enableenvironment.cc \
	Application/onoptimizerbatch.cc \
	Application/ondispatch.cc \
	Application/hashtoken.cc \
	Application/onoptimizerclosed.cc \
	Application/oninterrupt.cc \
	Application/ongreeting.cc \
	Application/sendwakeup.cc \
	Application/onworkerchallenge.cc \
	Application/onworkerremoved.cc \
	Application/onidlework.cc \
	Application/parsearguments.cc \
	TaskQueue/pop.cc \
	TaskQueue/updateprioritysum.cc \
	TaskQueue/insert.cc \
	TaskQueue/queue2.cc \
	TaskQueue/empty.cc \
	TaskQueue/taskqueue.cc \
	TaskQueue/queue1.cc \
	TaskQueue/remove.cc \
	TaskQueue/Task/failed.cc \
	TaskQueue/Task/overtake.cc \
	TaskQueue/Task/sequencedecrease.cc \
	TaskQueue/Task/sequence1.cc \
	TaskQueue/Task/failures.cc \
	TaskQueue/Task/task1.cc \
	TaskQueue/Task/task2.cc \
	TaskQueue/Task/clone.cc \
	TaskQueue/Task/canovertake.cc \
	TaskQueue/Task/sequence2.cc \
	TaskQueue/Task/operatorequal.cc \
	WorkerPool/workerpool.cc \
	WorkerPool/add.cc \
	WorkerPool/dataremoveworker.cc \
	WorkerPool/specifier.cc \
	WorkerPool/dataonworkerresponse.cc \
	WorkerPool/close.cc \
	WorkerPool/activate.cc \
	WorkerPool/dataonworkerclosed.cc \
	Optimizer/id.cc \
	Optimizer/optimizer1.cc \
	Optimizer/optimizer2.cc \
	Optimizer/onbatch.cc \
	Optimizer/operatorequal.cc \
	Optimizer/dataonclientdata.cc \
	Optimizer/response.cc \
	Worker/datasetfailure.cc \
	Worker/dataondata.cc \
	Worker/worker1.cc \
	Worker/send.cc \
	Worker/worker2.cc \
	OptimizerManager/optimizermanager.cc \
	OptimizerManager/onnewconnection.cc \
	OptimizerManager/find.cc \
	OptimizerManager/onconnectionclosed.cc \
	OptimizerManager/onoptimizerbatch.cc \
	OptimizerManager/fromdescriptor.cc \
	OptimizerManager/set.cc \
	OptimizerManager/listen.cc \
	Config/statics.cc \
	Config/initialize.cc \
	Config/config.cc \
	Config/instance.cc

noinst_HEADERS = \
	Application/application.ih \
	Application/application.hh \
	TaskQueue/taskqueue.ih \
	TaskQueue/taskqueue.hh \
	TaskQueue/Task/task.hh \
	TaskQueue/Task/task.ih \
	WorkerPool/workerpool.hh \
	WorkerPool/workerpool.ih \
	Optimizer/optimizer.ih \
	Optimizer/optimizer.hh \
	Worker/worker.ih \
	Worker/worker.hh \
	OptimizerManager/optimizermanager.ih \
	OptimizerManager/optimizermanager.hh \
	Config/config.ih \
	Config/config.hh

EXTRA_DIST = $(noinst_HEADERS)
