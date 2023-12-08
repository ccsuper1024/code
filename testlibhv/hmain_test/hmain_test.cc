/* @author chenchao 
    @date 
    @file 
    @brief 编写一个完整命令行的程序。
           功能：命令行解析
                配置文件解析
                信号处理
                日志，pid文件
                如果是服务端长时间运行后台程序，还需要看门狗(崩溃重启)
    */
#include <stdio.h>
#include <hv/hv.h>
#include <hv/hmain.h>
#include <hv/iniparser.h>
#include <iostream>

using std::endl;
using std::cout;
using std::cerr;
using std::cin;

typedef struct conf_ctx_s{
    IniParser* parser;
    int loglevel;
    int worker_processes;
    int worker_threads;
    int port;
} conf_ctx_t;
conf_ctx_t g_conf_ctx;

inline void conf_ctx_init(conf_ctx_t* ctx){
    ctx->parser = new IniParser;
    ctx->loglevel = LOG_LEVEL_DEBUG;
    ctx->worker_processes = 0;
    ctx->worker_threads = 0;
    ctx->port = 0;
}

//静态内联函数
static void print_version();    
static void print_help();
static int parse_confile(const char* confile);
static void worker_fn(void* userdata);

//short options
static const char options[] = "hvc:ts:dp:";
//long options
static const option_t long_options[] = {
    {'h',"help",        NO_ARGUMENT},
    {'v',"version",     NO_ARGUMENT},
    {'c',"confile",     REQUIRED_ARGUMENT},
    {'t',"test",        NO_ARGUMENT},
    {'s',"signal",      REQUIRED_ARGUMENT},
    {'d',"daemon",      NO_ARGUMENT},
    {'p',"port",        REQUIRED_ARGUMENT}
};
static const char detail_options[] = R"(        //原始字符串
    -h|--help               Print  this infomation
    -v|--version            Print version
    -c|--confile <confile>  Set configure file,deafault etc/{program}.conf
    -t|--test               Test configure file and exit
    -s|--signal <signale>   Send <signal> to process,
                            <signal>=[start,stop,restart,status,reload]
    -d|--daemon             Daemonize
    -p|--port <port>        Set listen port
)";
void print_version(){
    fprintf(stdout,"%s version %s\n",g_main_ctx.program_name,hv_compile_version());
}
void print_help(){
    fprintf(stdout,"Usage: %s [%s]\n",g_main_ctx.program_name,options);
    fprintf(stdout,"Options:\n%s\n",detail_options);
}
int parse_confile(const char* confile){
    int ret = g_conf_ctx.parser->LoadFromFile(confile);
    if(ret != 0){
        fprintf(stderr,"Load confile [%s] failed: %d\n",confile,ret);
        exit(-40);
    }

    //logfile
    std::string str = g_conf_ctx.parser->GetValue("logfile");
    if(!str.empty()){
        strncpy(g_main_ctx.logfile,str.c_str(),sizeof(g_main_ctx.logfile));
    }
    hlog_set_file(g_main_ctx.logfile);
    //loglevel
    str = g_conf_ctx.parser->GetValue("loglevel");
    if(!str.empty()){
        hlog_set_level_by_str(str.c_str());
    }

    //log_filesize
    str = g_conf_ctx.parser->GetValue("log_filesize");
    if(!str.empty()){
        hlog_set_max_filesize_by_str(str.c_str());
    }

    //log_fsync
    str = g_conf_ctx.parser->GetValue("log_fsync");
    if(!str.empty()){
        logger_enable_fsync(hlog,hv_getboolean(str.c_str()));
    }
    //first log here
    hlogi("%s version:%s",g_main_ctx.program_name,hv_compile_version());
    hlog_fsync();

    //worker_processes
    int worker_processes = 0;
#ifdef DEBUG
    //Disable multi-processes mode for debugging
    worker_processes = 0;
#else
    str = g_conf_ctx.parser->GetValue("worker_processes");
    if(str.size() != 0){
        if(strcmp(str.c_str(),"auto") == 0){
            worker_processes = get_ncpu();  //linux平台通过sysconf函数获得系统信息
            hlogd("worker_processes=ncpu=%d",worker_processes);
        }else{
            worker_processes = atoi(str.c_str());   // ascii to int
        }
    } 
#endif
    //#define LIMIT(lower, v, upper) ((v) < (lower) ? (lower) : (v) > (upper) ? (upper) : (v))
    g_conf_ctx.worker_processes = LIMIT(0,worker_processes,MAXNUM_WORKER_PROCESSES);
    //worker threads
    int worker_threads = 0;
    str = g_conf_ctx.parser->GetValue("worker_threads");
    if(str.size() != 0){
        if(strcmp(str.c_str() ,"auto") == 0){
            worker_threads = get_ncpu();
            hlogd("worker_threads=ncpu=%d",worker_threads);
        }else{
            worker_threads = atoi(str.c_str());
        }
    }
    g_conf_ctx.worker_threads =LIMIT(0,worker_threads,64);

    //port
    int port = 0;
    const char* szPort = get_arg("p");
    if(szPort){
        port = atoi(szPort);
    }
    if(port == 0){
        port = g_conf_ctx.parser->Get<int>("port");
    }
    if(port == 0){
        fprintf(stderr,"Please config listen port\n");
        exit(-10);
    }
    g_conf_ctx.port = port;
    hlogi("parse_confile('%s') OK",confile);
    return 0;
}
static void on_reload(void* userdata){
    hlogi("reload confile [%s]",g_main_ctx.confile);
    parse_confile(g_main_ctx.confile);
}

/* @brief @pram @return */
void test(int argc,char* argv[]){
    //g_main_ctx
    main_ctx_init(argc,argv);
    if(argc == 1){
        print_help();
        exit(10);
    }
    //int ret = parse_opt(argc,argv,options);
    int ret = parse_opt_long(argc,argv,long_options,ARRAY_SIZE(long_options));
    if(ret != 0){
        print_help();
        exit(ret);
    }

    //help
    if(get_arg("h")){
        print_help();
        exit(0);    //正常退出
    }
    //version
    if(get_arg("v")){
        print_version();
        exit(0);
    }
    //g_conf_ctx
    conf_ctx_init(&g_conf_ctx);
    const char* confile = get_arg("c");
    if(confile){
        strncpy(g_main_ctx.confile,confile,sizeof(g_main_ctx.confile));
    }
    parse_confile(g_main_ctx.confile);

    //test
    if(get_arg("t")){
        fprintf(stdout,"Test confile [%s] OK!\n",g_main_ctx.confile);
        exit(0);
    }

    //signal
    signal_init(on_reload);
    const char* signal = get_arg("s");
    if(signal){
        signal_handle(signal);
    }
#ifndef OS_UNIX
    //daemon
    if(get_arg("d")){
        //nochdir noclose
        int ret = daemon(1);
        if(ret != 0){
            fprintf(stderr,"daemon error: %d\n",ret);
            exit(-10);
        }
    }
#endif
    
    //pidfile
    create_pidfile();
    master_workers_run(worker_fn,(void*)(intptr_t)g_conf_ctx.port,
            g_conf_ctx.worker_processes,g_conf_ctx.worker_threads);
}
int main(int argc,char* argv[])
{
    test(argc,argv);
    return 0;
}

void worker_fn(void* userdata){
    long port = reinterpret_cast<long>(userdata);
    while(true){
        fprintf(stdout,"port=%ld pid=%ld tid=%ld\n",port,hv_getpid(),hv_gettid());
        hv_delay(60000);
    }
}

