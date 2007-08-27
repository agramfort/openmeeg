#define command_usage(usage) command_line::option((const char*)0,argc,argv,(const char*)0,usage)
#define command_option(name,defaut,usage) command_line::option(name,argc,argv,defaut,usage)

#ifdef WIN32
#define command_line_OS 2
#pragma warning( disable : 4530)    //MSVC standard library can't be inlined
#pragma warning( disable : 4996)    //MSVC warning C4996: declared deprecated
#else
#define use_color_terminal
#define command_line_OS 1
#endif

#include <cmath>
#include <cstdlib>
#include <iostream>

namespace command_line {
    
    #ifdef use_color_terminal
        const char t_normal[9]  = {0x1b,'[','0',';','0',';','0','m','\0'};
        const char t_red[11]    = {0x1b,'[','4',';','3','1',';','5','9','m','\0'};
        const char t_bold[5]    = {0x1b,'[','1','m','\0'};
        const char t_purple[11] = {0x1b,'[','0',';','3','5',';','5','9','m','\0'};
    #else
        const char t_normal[1]  = {'\0'};
        const char *const t_red = command_line::t_normal, *const t_bold = command_line::t_normal, *const t_purple = command_line::t_normal;
    #endif
    
    inline char uncase(const char x) { return (char)((x<'A'||x>'Z')?x:x-'A'+'a'); }
    inline float atof(const char *str) {
        float x=0,y=1;
        if (!str) return 0; else { std::sscanf(str,"%g/%g",&x,&y); return x/y; }
    }
    inline int strlen(const char *s) { if (s) { int k; for (k=0; s[k]; k++) ; return k; } return -1; }
    inline int strncmp(const char *s1,const char *s2,const int l) {
        if (s1 && s2) { int n=0; for (int k=0; k<l; k++) n+=abs(s1[k]- s2[k]); return n; }
        return 0;
    }
    inline int strfind(const char *s,const char c)
    {
        if (s) { 
            int l; for (l=command_line::strlen(s); l>=0 && s[l]!=c; l--) ;
            return l; 
        }
        return -1; 
    }
    inline int strncasecmp(const char *s1,const char *s2,const int l) {
        if (s1 && s2) { int n=0; for (int k=0; k<l; k++) n+=abs(uncase(s1[k])-uncase(s2[k])); return n; }
        return 0;
    }
    inline int strcmp(const char *s1,const char *s2) { 
        const int l1 = command_line::strlen(s1), l2 = command_line::strlen(s2);
        return command_line::strncmp(s1,s2,1+(l1<l2?l1:l2));
    }
    inline int strcasecmp(const char *s1,const char *s2) {
        const int l1 = command_line::strlen(s1), l2 = command_line::strlen(s2);
        return command_line::strncasecmp(s1,s2,1+(l1<l2?l1:l2));
    }
    inline const char* basename(const char *s)
    {
        return (command_line_OS!=2)?(s?s+1+command_line::strfind(s,'/'):NULL):(s?s+1+command_line::strfind(s,'\\'):NULL); 
    }
    
    inline const char* option(const char *const name, const int argc, char **argv,
                              const char *defaut, const char *const usage=NULL)
    {
        static bool first=true, visu=false;
        const char *res = NULL;
        if (first) { first=false; visu = command_line::option("-h",argc,argv,(const char*)NULL)!=NULL; }
        if (!name && visu) {
            std::fprintf(stderr,"\n %s%s%s",command_line::t_red,command_line::basename(argv[0]),command_line::t_normal);
            if (usage) std::fprintf(stderr," : %s",usage);
            std::fprintf(stderr," (%s, %s)\n\n",__DATE__,__TIME__);
        }
        if (name) {
            if (argc>0) {
                int k=0,i;
                while (k<argc && command_line::strcmp(argv[k],name)) k++;
                i=k;
                res=(k++==argc?defaut:(k==argc?argv[--k]:argv[k]));
                } else res = defaut;
                if (visu && usage) std::fprintf(stderr,"    %s%-8s%s = %-12s : %s%s%s\n",
                    command_line::t_bold,name,command_line::t_normal,res?res:"NULL",command_line::t_purple,usage,command_line::t_normal);
            }
            return res;
    }

    inline bool option(const char *const name, const int argc, char **argv,
                       const bool defaut, const char *const usage=NULL) {
        const char *s = command_line::option(name,argc,argv,(const char*)NULL);
        const bool res = s?(command_line::strcasecmp(s,"false") && command_line::strcasecmp(s,"off") && command_line::strcasecmp(s,"0")):defaut;
        command_line::option(name,0,NULL,res?"true":"false",usage);
        return res;
    }

    inline int option(const char *const name, const int argc, char **argv,
                      const int defaut, const char *const usage=NULL) {
        const char *s = command_line::option(name,argc,argv,(const char*)NULL);
        const int res = s?std::atoi(s):defaut;
        char tmp[256];
        std::sprintf(tmp,"%d",res);
        command_line::option(name,0,NULL,tmp,usage);
        return res;
    }

    inline char option(const char *const name, const int argc, char **argv,
               const char defaut, const char *const usage=NULL) {
        const char *s = command_line::option(name,argc,argv,(const char*)NULL);
        const char res = s?s[0]:defaut;
        char tmp[8];
        tmp[0] = res;
        tmp[1] ='\0';
        command_line::option(name,0,NULL,tmp,usage);
        return res;
    }

    inline double option(const char *const name, const int argc, char **argv,
             const double defaut, const char *const usage=NULL) {
        const char *s = command_line::option(name,argc,argv,(const char*)NULL);
        const double res = s?command_line::atof(s):defaut;
        char tmp[256];
        std::sprintf(tmp,"%g",res);
        command_line::option(name,0,NULL,tmp,usage);
        return res;
    }

}

#ifdef SRCDIR
#define SP_STRINGIFY(s)  #s
#define SP_EVAL(s) SP_STRINGIFY(s)
#define SRCPATH(s) (SP_EVAL(SRCDIR)"/"s)
#else
#define SRCPATH(s) ("")
#endif