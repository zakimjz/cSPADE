#include <errno.h>
#include "Itemset.h"
#include "spade.h"

using namespace std;
extern char print_tidlist;

Itemset::Itemset(int it_sz, int ival_sz, int nclass){
   //cout << "ITALLOC " << MEMUSED;
   theItemset = new Array(it_sz);
   if (theItemset == NULL){
      perror("memory:: Itemset");
      exit(errno);
   }

   theIval = new Array(ival_sz);
   if (theIval == NULL){
      perror("memory:: Ival");
      exit(errno);
   }

   theMcnt = NULL;
   if (ival_sz > 0){
      theMcnt = new vector<ullint>;
      if (theMcnt == NULL){
         perror("memory:: Ival");
         exit(errno);
      }
   }

   //for (int i=0; i < ival_sz; i++)
   //   theIval[i] = NULL;
   theSupport = 0;
   clsSup = new int[nclass];
   for (int i=0; i < nclass; i++) clsSup[i] = 0;
   //for (int i=0; i < ival_sz; i++) (*theMcnt)[i] = 0;


   MEMUSED += sizeof(Itemset)+nclass*sizeof(int);
   //cout << " -- " << MEMUSED << endl;
}

Itemset::~Itemset(){
   //cout << "ITDEL " << MEMUSED;
   if (theItemset) delete theItemset;
   if (theIval) delete theIval;
   if (theMcnt) delete theMcnt;
   theItemset = NULL;
   theSupport = 0;
   MEMUSED -= sizeof(Itemset);
   //cout << " -- " << MEMUSED << endl;
}

int Itemset::compare(Itemset& ar2)
{
   int len;
   if (size() <= ar2.size()) len = size();
   else len = ar2.size();
   for(int i=0; i < len; i++){
      if ((*theItemset)[i] > (*ar2.theItemset)[i]) return 1;
      else if ((*theItemset)[i] < (*ar2.theItemset)[i]) return -1;
   }
   if (size() < ar2.size()) return -1;
   else if (size() > ar2.size()) return 1;
   else return 0;
}

//len must be less than length of both Itemsets
int Itemset::compare(Itemset& ar2, int len)
{
   for(int i=0; i < len; i++){
      if ((*theItemset)[i] > (*ar2.theItemset)[i]) return 1;
      else if ((*theItemset)[i] < (*ar2.theItemset)[i]) return -1;
   }
   return 0;
}
int Itemset::compare(Array& ar2, int len)
{
   for(int i=0; i < len; i++){
      if ((*theItemset)[i] > ar2[i]) return 1;
      else if ((*theItemset)[i] < ar2[i]) return -1;
   }
   return 0;
}

int Itemset::compare(Itemset& ar2, int len, unsigned int bvec)
{
   int pos = 0;
   int it;
   for(int i=0; i < len; i++){
      while (!GETBIT(bvec, pos)){
         pos ++;
      }
      it = (*theItemset)[pos++];
      if (it > (*ar2.theItemset)[i]) return 1;
      else if (it < (*ar2.theItemset)[i]) return -1;
   }
   return 0;
}

int Itemset::subsequence(Itemset * ar)
{
   int i,j;
   if (size() > ar->size()) return 0;
   int start = 0;
   for(i=0; i < size(); i++){
      for(j=start; j < ar->size(); j++){
         if ((*theItemset)[i] == (*ar->theItemset)[j]){
            start = j+1;
            break;
         }
      }
      if (j >= ar->size()) return 0;
   }
   return 1;
}

ostream& operator << (ostream& outputStream, Itemset& itemset){
   outputStream << "ITEM: ";
   outputStream << *itemset.theItemset;
   outputStream << "(" << itemset.theSupport << ")";
   outputStream << "\n";
   return outputStream;
}

void Itemset::print_seq(int itempl)
{
   int i;
   int sz = size();
   //int templ = itempl;
   //int mask = 1 << (size()-1);
   cout << (*theItemset)[0] << " ";
   for (i=1; i < sz-1; i++){
      //if (templ && mask) cout << "->";
      //templ = templ << 1;
      if (GETBIT(itempl,sz-1-i))
         cout << "-> ";
      cout << (*theItemset)[i] << " ";
   }
   if (GETBIT(itempl,sz-1-i))
      cout << "-> ";
   cout << (*theItemset)[sz-1] << " ";
   cout << "-- " << theSupport;

   if (use_class)
      for (i=0; i < NUMCLASS; i++)
         cout << " " << clsSup[i];
   cout << " ";
   if (print_tidlist){
      cout << " -- ";
      print_idlist();
   }
   cout << endl;
}

void Itemset::print_idlist()
{
   int i, cid, cnt;

   if (theIval && theIval->size() > 0){
      for (i=0; i < theIval->size(); ++i){
         cout << " " << (*theIval)[i];
         if (count_multiple && i%2 == 0)
            cout << "(" << (*theMcnt)[i/2] << ")";
      }
      //cout << endl;
   }
   //    if (theIval && theIval->size() > 0){
//       cid = (*theIval)[0];
//       cnt = 0;
//       for (i=0; i < theIval->size(); ){
//          if (cid == (*theIval)[i]){
//             cnt++;
//             i+=2;
//          }
//          else{
//             cout << cid << " " << cnt << " ";
//             cid =(*theIval)[i];
//             cnt = 0;
//          }
//       }
//       cout << cid << " " << cnt;
//    }

}

