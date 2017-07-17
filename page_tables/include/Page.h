#ifndef PAGE_H
#define PAGE_H

/*
* This class defines a page. Each page has a unique pageNumber across all pages in the system.
* Valid is true when the page is loaded into memory, false otherwise.
* R is used for clock replacement and is set to 1 when accessed from ptrace.
* lastTimeAccessed corresponds to the line of ptrace that this page was last accessed from. If loaded during the inital load
* this is set to 0.
*/
class Page
{
public:
  int pageNumber;
  bool valid;
  bool R;
  unsigned long lastTimeAccessed;

  Page(int id);
  //the load function sets valid to 1, R to 1, and lastTimeAccessed to accessTime
  void load(unsigned long accessTime);
};

#endif