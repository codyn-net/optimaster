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
	Config/instance.cc \
	Config/statics.cc \
	Config/config.cc \
	Config/initialize.cc \
	Worker/worker1.cc \
	Worker/datasetfailure.cc \
	Worker/worker2.cc \
	Worker/send.cc \
	Worker/dataondata.cc \
	Optimizer/response.cc \
	Optimizer/optimizer1.cc \
	Optimizer/id.cc \
	Optimizer/onbatch.cc \
	Optimizer/optimizer2.cc \
	Optimizer/dataonclientdata.cc \
	Optimizer/operatorequal.cc \
	Optimizer/client.cc \
	WorkerPool/activate.cc \
	WorkerPool/dataremoveworker.cc \
	WorkerPool/dataonworkerclosed.cc \
	WorkerPool/add.cc \
	WorkerPool/specifier.cc \
	WorkerPool/close.cc \
	WorkerPool/dataonworkerresponse.cc \
	WorkerPool/workerpool.cc \
	OptimizerManager/find.cc \
	OptimizerManager/listen.cc \
	OptimizerManager/fromdescriptor.cc \
	OptimizerManager/onconnectionclosed.cc \
	OptimizerManager/optimizermanager.cc \
	OptimizerManager/set.cc \
	OptimizerManager/onoptimizerbatch.cc \
	OptimizerManager/onnewconnection.cc \
	OptimizerManager/destructor.cc \
	Application/parsearguments.cc \
	Application/onworkerchallenge.cc \
	Application/destructor.cc \
	Application/registerworker.cc \
	Application/ondispatch.cc \
	Application/oninterrupt.cc \
	Application/run.cc \
	Application/onoptimizerclosed.cc \
	Application/onworkerremoved.cc \
	Application/hashtoken.cc \
	Application/onworkerresponse.cc \
	Application/ongreeting.cc \
	Application/onidlework.cc \
	Application/application.cc \
	Application/enableenvironment.cc \
	Application/onoptimizerbatch.cc \
	Application/sendwakeup.cc \
	TaskQueue/empty.cc \
	TaskQueue/updateprioritysum.cc \
	TaskQueue/remove.cc \
	TaskQueue/taskqueue.cc \
	TaskQueue/insert.cc \
	TaskQueue/queue1.cc \
	TaskQueue/queue2.cc \
	TaskQueue/pop.cc \
	TaskQueue/Task/clone.cc \
	TaskQueue/Task/task1.cc \
	TaskQueue/Task/failures.cc \
	TaskQueue/Task/overtake.cc \
	TaskQueue/Task/sequencedecrease.cc \
	TaskQueue/Task/task2.cc \
	TaskQueue/Task/failed.cc \
	TaskQueue/Task/canovertake.cc \
	TaskQueue/Task/sequence1.cc \
	TaskQueue/Task/operatorequal.cc \
	TaskQueue/Task/sequence2.cc

noinst_HEADERS = \
	Config/config.hh \
	Config/config.ih \
	Worker/worker.ih \
	Worker/worker.hh \
	Optimizer/optimizer.hh \
	Optimizer/optimizer.ih \
	WorkerPool/workerpool.hh \
	WorkerPool/workerpool.ih \
	OptimizerManager/optimizermanager.hh \
	OptimizerManager/optimizermanager.ih \
	Application/application.hh \
	Application/application.ih \
	TaskQueue/taskqueue.hh \
	TaskQueue/taskqueue.ih \
	TaskQueue/Task/task.ih \
	TaskQueue/Task/task.hh

EXTRA_DIST = $(noinst_HEADERS)
