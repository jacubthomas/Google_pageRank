#include "page.h"
using namespace std;

// default Page constructor
Page::Page()
{
    id = 0;
    url = "";
    rank = 0;
};

// Overloaded Page constructor
Page::Page( int val, string site )
{
    id = val;
    url = site;
    rank = 0.0;
}

// deallocates links vector
Page::~Page()
{
    while( !(links_empty()) )
    {
    links_pop_back();
    }
}

int Page::get_id()
{
    return id;
}

void Page::set_id( int val )
{
    id = val;
}

string Page::get_url()
{
    return url;
}

void Page::set_url( string site )
{
    url = site;
}

double Page::get_rank()
{
    return rank;
}

void Page::set_rank( double val )
{
    rank = val;
}

int Page::links_front()
{
    return links.front();
}

int Page::links_back()
{
    return links.back();
}

void Page::links_push_back( int site_id )
{
    links.push_back( site_id );
}
int Page::links_pop_back()
{
    int back = links_back();
    links.pop_back();
    return back;
}
bool Page::links_empty()
{
    return links.empty();
}

int Page::links_size()
{
    int count = 0;
    for( unsigned int i = 0; i < links.size(); i++ )
    {
        count++;
    }
    return count;
}

int Page::links_at( int idx )
{
    return links.at(idx);
}
