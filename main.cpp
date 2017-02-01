/*
Propositions are identified by English alphabet letters in lowercase form;
Input format:
Logical operators are set in the form of !, ^, V, ->, <=> (Negation, Conjuction, Disjunction, Implication, Equivalence);
Each two propositions operation must be included inside parentheses;
Each two block operation must be included inside parentheses;
*/

#include<iostream>
#include<fstream>
#include<map>
#include<cctype>
using namespace std;

//max length of input string
#define MAX_N 100

//max number of propositions
#define MAX_P 10

int blks_nr;
struct
{
    int start;  //the position in string of '('
    int finish; //the position in string of ');
}block[MAX_P];  //block[i], where i is the block with number i

string sir, alphabet; map<char,int> var;
int per[MAX_N], RESULT[100], subm[MAX_N];


//file input
void read()
{
    ifstream inputFile;
    inputFile.open("project.in");

    if (inputFile.is_open())
        while (!inputFile.eof())
            inputFile >> sir;
    inputFile.close();

    /*
     for(int z=0; z<sir.length(); z++)
        cout << sir[z];
    cout << endl;
    cout << sir.length();
    */
}


//negation operator transforming
inline int non(int x)
{
    return (x==0) ? 1:0;
}


//applying the operators on values
int calculus(int a1, char op, int a2)
{
    //conjuction
    if(op=='^')
    {
        if(a1==1 && a2==1)
            return 1;
        else
            return 0;
    }

    //disjunction
    if(op=='V')
    {
        if(a1==0 && a2==0)
            return 0;
        else
            return 1;
    }

    //implication
    if(op=='>')
    {
        if(a1==1 && a2==0)
            return 0;
        else
            return 1;
    }

    //equivalence
    if(op=='=')
    {
        if((a1==1 && a2==1)||(a1==0 && a2==0))
            return 1;
        else
            return 0;
    }
}

// short block = block containing only one pair of parentheses
bool short_block(int m)
{
    if(block[m].finish - block[m].start <=8)
        return true;
    else
        return false;
}



// ( p <=> !q ) block type proccesing, there aren't another parentheses inside
void compute_short_block(int m, int &a1, char &op, int &a2)
{

    if(sir[block[m].start+1]=='!') //if first proposition got negation
    {
        a1=non(var[sir[block[m].start+2]]);
        switch(sir[block[m].start+3])
        {
             case '^':
                op='^'; //conjuction operator
                if(sir[block[m].start+4]=='!')
                    a2=non(var[sir[block[m].start+5]]);
                else
                    a2=var[sir[block[m].start+4]];
                break;

              case 'V':
                op='V'; //disjunction operator
                if(sir[block[m].start+4]=='!')
                    a2=non(var[sir[block[m].start+5]]);
                else
                    a2=var[sir[block[m].start+4]];
                break;

            case '-':
                op = '>';  //implication operator
                if(sir[block[m].start+5]=='!')
                    a2 = non(var[sir[block[m].start+6]]);
                else
                    a2 = var[sir[block[m].start+5]];
                break;

            case '<':
                op = '='; //equivalence operator
                if(sir[block[m].start+6]=='!')
                    a2=non(var[sir[block[m].start+7]]);
                else
                    a2=var[sir[block[m].start+6]];
                break;

            default:
                printf("Error! Undefined operator!\n");
        }
    }
    else
    {
        a1=var[sir[block[m].start+1]];
        switch(sir[block[m].start+2])
        {
             case '^':
                op = '^'; //conjuction operator
                if(sir[block[m].start+3]=='!')
                    a2=non(var[sir[block[m].start+4]]);
                else
                    a2=var[sir[block[m].start+3]];
                break;

            case 'V':
                op ='V'; //disjunction operator
                if(sir[block[m].start+3]=='!')
                    a2=non(var[sir[block[m].start+4]]);
                else
                    a2=var[sir[block[m].start+3]];
                break;

            case '-':
                op = '>';  //implication operator
                if(sir[block[m].start+4]=='!')
                    a2=non(var[sir[block[m].start+5]]);
                else
                    a2=var[sir[block[m].start+4]];
                break;

            case '<':
                op = '='; //equivalence operator
                if(sir[block[m].start+5]=='!')
                    a2=non(var[sir[block[m].start+6]]);
                else
                    a2=var[sir[block[m].start+5]];
                break;

            default:
                printf("Error! Undefined operator!\n");
        }
    }

}


// the block contains more than one pair of parentheses
void compute_long_block(int m, int &a1, char &op, int &a2)
{
    a1 = RESULT[per[block[m].start+1]];

    if(sir[block[per[block[m].start+1]].finish+1] == '<')   //equivalence
        op = '=';
    else
        if(sir[block[per[block[m].start+1]].finish+1] == '-') //implication
            op = '>';
        else
            op = sir[block[per[block[m].start+1]].finish+1];

    a2 = RESULT[per[block[m].finish-1]];
}

void solve()
{
    for(int i=1; i<=blks_nr; i++)
    {
        int p1, p2;
        char _operator;
        if(short_block(i))
            compute_short_block(i, p1, _operator, p2);
        else
            compute_long_block(i, p1, _operator, p2);

        //cout << p1 << _operator << p2 << "R:" << calculus (p1, _operator, p2) << endl;


        RESULT[i] = calculus (p1, _operator, p2);

    }
    //cout << blks_nr;
    cout << "Solution: " << RESULT[blks_nr] << endl;
    //cout << endl;
}

int subsets_number;
//generating all 1 and 0 subsets for the number of propositions
void btrack(int k)
{
    if(alphabet.length() == k) //if we got one complete subset
    {
        int i;
        for(i=0; i<alphabet.length(); i++)  //assign the 1 or 0 for each proposition
            var[alphabet[i]]=subm[i];

        /*
        for(i=0; i<alphabet.length(); i++)
            cout << alphabet[i] << " = " << subm[i] << endl;
        cout << endl;
        */
        //

        //subsets_number++;

        solve();
    }
    else
    {
        subm[k]=-1;
        while(subm[k]<1)
        {
            subm[k]++;
            btrack(k+1);
        }
    }
}


//check if blocks are made from i to j positions
bool unpaired(int i, int j)
{
    for(int x=i; x<=j; x++)
        if(!per[x] && (sir[x]=='(' || sir[x]==')')) // if there's left some parenthesis unmarked
        {
            return true;
            break;
        }
    return false;
}


//each parenthesis must have its pair
void create_match()
{
    int i,ok,a;
    for(i=0; i<sir.length()-1; i++)
    {
        if (    // for the rightest open parenthesis
                (sir[i]=='(' && per[i]==0 && sir[i+1]!='(')   ||
                // for left parenthesis closest to the marked in the right
                (sir[i]=='(' && per[i]==0 && sir[i+1]== '(' && per[i+1]!=0)
                                                                                )
        {
            a=i;
            ok=1;
            //cout << a << endl;
        }

        if( sir[i]==')' && per[i]==0)
        {
            if(ok==1)
            {
                blks_nr++;
                per[i]=blks_nr;
                per[a]=blks_nr;

                //cout << per[a] << " " << per[i]<<endl;

                block[blks_nr].start=a;
                block[blks_nr].finish=i;
            }
            ok=0;

            //cout << block[i].start << " " <<block[i].finish << "\n";
        }

        //if all the blocks are made
        //we complete the last one, from 0 to sir.length()-1
        if(!unpaired(1, sir.length()-2) && i==sir.length()-2 && ok)
        {
            //cout <<"x";
            blks_nr++;
            i++;
            per[i]=blks_nr;
            per[a]=blks_nr;
            block[blks_nr].start=a;
            block[blks_nr].finish=i;
        }

    }


   // cout << "text";
}


// check for character inside string
bool found (char c)
{
    for(int j=0; j<alphabet.length(); j++)
            if(alphabet[j] == c)
                    return true;
    return false;
}


//the array will contain all the once added propositions from string
//in the order of string processing
void buildPropositionsArray()
{
    for(int i=0; i<sir.length(); i++)
        if(islower(sir[i]) && !found(sir[i]))
            alphabet.push_back(sir[i]);

    /*
    for(int z=0; z<alphabet.length(); z++)
        cout << alphabet[z] << " ";
    cout << endl;
    cout << alphabet.length();
    */
}



int main()
{
    read();
    buildPropositionsArray();




    //transformare();
   // generare();
    //computing the opened-closed parentheses couples
    // e .g
    //  ( ( C C C ) C C ( C C C ) )
    //  3 1 0 0 0 1  0 0 2 0 0 0 2 3




    while(unpaired(0, sir.length()-1))
          create_match();



    //int RESULT[blks_nr+1];


        /*
        create_match();

        create_match();

        create_match();

        create_match();

        create_match();

        create_match();

        create_match();

        */

        /*
        cout << endl;
        for(int i=0; i<sir.length( ); i++)
            cout << per[i]<< " ";
        cout<<endl;
        */

        /*
        for(int i=1; i<=blks_nr; i++)
            cout << i << " " << block[i].start << " " << block[i].finish << endl;
        */

        /*
        int pd=0, pi=0;
        for(int i=0; i<sir.length(); i++)
        {

            if(sir[i]=='(')
               pd++;
            if(sir[i]==')')
                pi++;

        }
        cout << pd << " " <<pi;
        */



    /*
    var['p'] = 0;
    var['q'] = 1;
    var['r'] = 1;
    var['s'] = 0;
    */

    /*
    RESULT[1] = 1;
    RESULT[2] = 0;
    RESULT[3] = 1;
    RESULT[4] = 1;
    */

   // solve();

    /*
    int a1, a2; char semn;
    compute_short_block(2, a1, semn, a2);
    compute_long_block(5, a1, semn, a2);
    cout << a1 << semn <<a2 << endl;
    */

    /*
      for(int t=0; t<sir.length(); t++)
        cout << per[t] << " ";
    */

        /*
        for(int t=0; t<w; t++)
        {
            cout << cnt[w].x <<< cnt[w].y << "\n";
        }
        */

    //////////////////////////////////////////////////


    btrack(0);
    //cout << subsets_number;

    return 0;
}

