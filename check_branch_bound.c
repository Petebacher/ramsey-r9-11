/* Generalised exact checker. Usage: gcheck <file> <K> <T>
   Decides: does the graph contain K_K ? does it contain an independent set of size T ?
   Uses Tomita-style branch and bound with colouring bound, decision mode (stops at first hit).
   Prints omega>=K? and alpha>=T? plus exact omega/alpha when run with K=0/T=0 (exact mode). */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#define MAXN 640
#define W 10
typedef struct { uint64_t w[W]; } bs;
static int n;
static bs adjm[MAXN], cadjm[MAXN];
static bs *G;
static int best, target, found; static long long nodes;
static inline int bempty(const bs *b){for(int k=0;k<W;k++) if(b->w[k]) return 0; return 1;}
static void expand(bs P,int sz){
  nodes++;
  int order[MAXN],col[MAXN],m=0,c=0; bs U=P;
  while(!bempty(&U)){ c++; bs Q=U;
    while(!bempty(&Q)){ int v=-1; for(int k=0;k<W;k++) if(Q.w[k]){v=k*64+__builtin_ctzll(Q.w[k]);break;}
      Q.w[v>>6]&=~(1ULL<<(v&63)); U.w[v>>6]&=~(1ULL<<(v&63));
      for(int k=0;k<W;k++) Q.w[k]&=~G[v].w[k];
      order[m]=v; col[m]=c; m++; } }
  for(int i=m-1;i>=0;i--){
    if(sz+col[i]<=best) return;
    int v=order[i]; bs NP; for(int k=0;k<W;k++) NP.w[k]=P.w[k]&G[v].w[k];
    if(bempty(&NP)){ if(sz+1>best){best=sz+1; if(target&&best>=target){found=1;return;}} }
    else { expand(NP,sz+1); if(found) return; }
    P.w[v>>6]&=~(1ULL<<(v&63)); }
}
static int maxclique(bs *g,int tgt,int *fnd){ G=g; best=tgt?tgt-1:0; target=tgt; found=0; nodes=0;
  bs all; memset(&all,0,sizeof all); for(int i=0;i<n;i++) all.w[i>>6]|=1ULL<<(i&63);
  expand(all,0); *fnd=found; return best; }
int main(int argc,char**argv){
  if(argc<4){fprintf(stderr,"usage: gcheck file K T (0 = exact)\n");return 2;}
  int K=atoi(argv[2]),T=atoi(argv[3]);
  FILE*f=fopen(argv[1],"r"); if(!f){perror("open");return 2;}
  static int a[MAXN*MAXN]; int cnt=0,ch;
  while((ch=fgetc(f))!=EOF) if(ch=='0'||ch=='1'){ if(cnt>=MAXN*MAXN){fprintf(stderr,"too big\n");return 2;} a[cnt++]=ch-'0'; }
  fclose(f); n=0; while(n*n<cnt) n++; if(n*n!=cnt){fprintf(stderr,"not square\n");return 2;}
  int bad=0;
  for(int i=0;i<n;i++)for(int j=0;j<n;j++){
    if(a[i*n+j]!=a[j*n+i]) bad++; if(i==j&&a[i*n+j]) bad++;
    if(i!=j&&a[i*n+j]) adjm[i].w[j>>6]|=1ULL<<(j&63);
    if(i!=j&&!a[i*n+j]) cadjm[i].w[j>>6]|=1ULL<<(j&63); }
  printf("n=%d asym_or_diag_errors=%d\n",n,bad);
  int fnd; int om=maxclique(adjm,K,&fnd);
  if(K) printf("K_%d present: %s  (nodes=%lld)\n",K,fnd?"YES":"no",nodes); else printf("omega exact=%d (nodes=%lld)\n",om,nodes);
  int al=maxclique(cadjm,T,&fnd);
  if(T) printf("independent %d present: %s  (nodes=%lld)\n",T,fnd?"YES":"no",nodes); else printf("alpha exact=%d (nodes=%lld)\n",al,nodes);
  int pass=(bad==0)&&K&&T&&!fnd; /* fnd is from alpha run */
  /* recompute clique result for verdict */
  int f2; maxclique(adjm,K,&f2);
  pass = (bad==0)&&K&&T&&!f2&&!fnd;
  printf("%s\n", K&&T ? (pass?"PASS":"FAIL") : "EXACT-MODE");
  return 0; }
