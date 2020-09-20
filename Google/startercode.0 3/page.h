#ifndef PAGE_H
#define PAGE_H
#include <string>
using std::string;
#include<vector>
#include<cmath>
using std::vector;
class Page
{
    public:
    Page();
    Page( int val, string site );
    ~Page();
    // setters and getters
    int get_id();
    void set_id( int val );
    string get_url();
    void set_url( string site );
    double get_rank();
    void set_rank( double val );
    
    int links_front();
    int links_back();
    void links_push_back( int site_id );
    int links_pop_back();
    bool links_empty();
    int links_size();
    int links_at( int idx );
    
    // Data members: id, url, pagerank, links , .. etc
    private:
    int id;
    string url;
    double rank;
    vector<int> links;
};  
#endif
