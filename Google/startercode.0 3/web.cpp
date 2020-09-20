#include "web.h"

// using default web constructor
Web::Web()
{
}

// web destructor deallocates memory of Pages array
Web::~Web()
{
    delete[] Pages;
}

/*
 reads in and parse graph from a file.
 @filename: is the name of the file describing the webgraph.
 @return: true in success , and false in failure.
*/
bool Web::read_graph(const char *filename)
{
    // opens file with check
    ifstream iFile(filename);
    if( iFile.fail() )
    {
        cout << "Input file failed to open" << endl;
        return false;
    }
    // retrieves number of pages with check
    int num_pages;
    iFile >> num_pages;
    if( iFile.fail() )
    {
        cout << "Input file failed to grab num_pages" << endl;
        return false;
    }
    
    // input loop setup
    int count = 0;
    string dummy;
    getline( iFile, dummy );
    
    // dynamically allocate page array
    Pages = new Page[ num_pages ];
    
    // 1 loop per page
    while( count < num_pages )
    {
        int val;
        string url;
        double rank;
        int ids;
        
        // get Page Id, and check for bad input
        if ( iFile >> val )
        {
        Pages[count].set_id( val );
        }
        else
        {
            cout << "Input file failed to grab id" << endl;
            return false;
        }
        // move stream to newline after int >>
        getline( iFile, dummy );
        
        // get page url & rank and check for bad input
        if ( iFile >> url >> rank )
        {
        Pages[count].set_url( url );
        Pages[count].set_rank( rank );
        }
        else
        {
            cout << "Input file failed to grab url/rank" << endl;
            return false;
        }
        
        // move stream to new line after double >>
        getline( iFile, dummy );
        
        // get whole line of Page links for stringstreaming and check
        string linksline;
        if ( !(getline( iFile, linksline )) )
        {
            cout << "No links section " << endl;
            return false;
        }
        
        // allocate ss with line of page links and push onto vector
        stringstream ss(linksline);
        while ( ss >> ids )
        {
            Pages[count].links_push_back(ids);
        }
        
        //increment count for page processed
        ++count;
    }
        
    // total number of pages
    setwebsize(count);
    
    // close input file stream and return true if 0 bad inputs
    iFile.close();
    return true;
}

bool Web::write_graph(const char *filename)
{
    // open output stream and check
    ofstream oFile( filename );
    if( oFile.fail() )
    {
        cout << "Output file failed to open" << endl;
        return false;
    }

    // write total number of pages at top of page and check
    int num_pages = getwebsize();
    if( !( oFile << num_pages << '\n') )
    {
        cout << "Output file failed to write num_pages" << endl;
    }
    
    // count to limit loop writes to number of pages
    int count = 0;
    while( count < num_pages )
    {
        // write one page and check
        if( !(oFile << Pages[count].get_id() << '\n' <<
        '\t' << Pages[count].get_url() << '\n' <<
        '\t' << Pages[count].get_rank() << '\n' <<
        '\t') )
        {
            cout << "Output file failed to write" << endl;
            return false;
        }

        // write links one at a time from links vector and check
        // use index so as to write links from front to back of vector
        int idx = 0;
        while( idx < Pages[count].links_size() )
        {
            
            if( !( oFile << Pages[count].links_at(idx) << " ") )
            {
                cout << "Output file failed to write links" << endl;
                return false;
            }
            idx++;
        }
        // new line for new page
        oFile << endl;
        
        // increment count for page written
        count++;
    }
    
    // close output file stream
    oFile.close();
    
    // close if file written entirely without failure
    return true;
}

void Web::calculate_rank(int S,int N)
{
    // S == Iterations , N == # of Surfers
    int start_distro = N / _size;
    int* crawlers = new int[N];
    
    // dynamic array of pages to assist in pr calculation
    int* crawlCount = new int[_size];
    
    // flush crawlCount with 0s
    for ( int i = 0; i < _size; i++ )
    {
        crawlCount[i] = 0;
    }
    
    // evenly distribute crawlers to start page positions
    int k = 0;
    for( int i = 0; i < start_distro; i++)
    {
        for( int j = 0; j < _size; j++ )
        {
            crawlers[k] = Pages[j].get_id();
            crawlCount[j]++;
            k++;
        }
    }
    // make 'em crawl
    for( int i = 0; i < S; i++ )
    {
        for( int j = 0; j < N; j++ )
        {
            // size of specific links vector for the page the crawler is on
            int this_size = Pages[ crawlers[j] ].links_size();
            
            // random index within bounds of links vector of the current page
            int rand_idx = rand() % this_size;
            
            // value at random index
            int rand_val = Pages[ crawlers[j] ].links_at( rand_idx );
            
            // decrement crawlCount for current page
            crawlCount[ crawlers[j] ]--;
            
            // move crawler to next new page
            crawlers[j] = rand_val;
            
            // increment crawlCount for current page
            crawlCount[ crawlers[j] ]++;
        }
    }
    
    // calculate page ranks
    for( int i = 0; i < _size; i++ )
    {
        double new_rank = abs(crawlCount[i] / (double) N );
         Pages[i].set_rank( new_rank );
    }
    delete[] crawlers;
    delete[]crawlCount;
}


int Web::getwebsize()
{
    return _size;
}

void Web::setwebsize( int num_pages )
{
    _size = num_pages;
}

// ./pagerank graph_rep.txt graph_out.txt 1000 16000
// ./pagerank graph_20_1_random.txt graph_out.txt 10000 16000
// ./pagerank graph_30_0.5_random.txt graph_out.txt 10000 16000
// run graph_rep.txt graph_output.txt 1000 16000

// valgrind --tool=memcheck --leak-check=yes
