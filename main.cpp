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
int subm[MAX_N], RESULT[MAX_N];

//  sir: ( ( C C C ) C C ( C C C ) )
//  per: 3 1 0 0 0 1  0 0 2 0 0 0 2 3
int per[MAX_N];

//file input
void read()
{
    ifstream inputFile;
    inputFile.open("project.in");

    if (inputFile.is_open())
        while (!inputFile.eof())
            inputFile >> sir;
    inputFile.close();
}


//negation operator transforming
int non(int x)
{
    return (x==0) ? 1:0;
}


//applying the operators on values
int calculus(int m, int a1, char op, int a2)
{
    //single proposition block
    if (op == 'x')
    {
        if(sir[block[m].start+1] == '!')
            return non(var[sir[block[m].start+2]]);
        else
            return var[sir[block[m].start+1]];
    }

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


    //single proposition block
    if(sir[block[m].start+2] == ')' || sir[block[m].start+3] == ')')
    {
        op = 'x';
        return;
    }

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

        RESULT[i] = calculus (i, p1, _operator, p2);

        for(int t=block[i].start; t<=block[i].finish; t++)
            cout << sir[t];
        cout << " " << RESULT[i] << " ";

    }
    cout << endl;
}

//generating all 1 and 0 subsets for the number of propositions
void btrack(int k)
{
    if(alphabet.length() == k) //if we got one complete subset
    {
        int i;
        for(i=0; i<alphabet.length(); i++)  //assign the 1 or 0 for each proposition
        {
            var[alphabet[i]]=subm[i];
            cout << alphabet[i] << "=" << subm[i] << " ";
        }
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
        }

        if( sir[i]==')' && per[i]==0)
        {
            if(ok==1)
            {
                blks_nr++;
                per[i]=blks_nr;
                per[a]=blks_nr;
                block[blks_nr].start=a;
                block[blks_nr].finish=i;
            }
            ok=0;
        }

        //if all the blocks are made
        //we complete the last one, from 0 to sir.length()-1
        if(!unpaired(1, sir.length()-2) && i==sir.length()-2 && ok)
        {
            blks_nr++;
            i++;
            per[i]=blks_nr;
            per[a]=blks_nr;
            block[blks_nr].start=a;
            block[blks_nr].finish=i;
        }

    }

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

}

int main()
{
    read();
    buildPropositionsArray();

    while(unpaired(0, sir.length()-1))
          create_match();

    btrack(0);

    return 0;
}

