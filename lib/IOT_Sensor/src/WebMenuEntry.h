#ifndef WEB_MENU_ENTRY_H
#define WEB_MENU_ENTRY_H

class  WebMenuEntry{
    public:
    // the key of the entry
    const char* Key=new char[20];
    // text written in the Menu
    const char* Name=new char[50];
    // URI of page to navigate
    const char* Page=new char[255];
    // Page content is provided internally by library or requested by callback
    bool IsCallbackPage=true;

    WebMenuEntry();
    WebMenuEntry(const char* key,const char* name,const char* page,bool isCallback=true){Key=key;Name=name;Page=page;IsCallbackPage=isCallback;}
};
#endif
#ifndef WEB_MENU_ENTRY_H
#define WEB_MENU_ENTRY_H

class  WebMenuEntry{
    public:
    // the key of the entry
    const char* Key=new char[20];
    // text written in the Menu
    const char* Name=new char[50];
    // URI of page to navigate
    const char* Page=new char[255];
    // Page content is provided internally by library or requested by callback
    bool IsCallbackPage=true;

    WebMenuEntry();
    WebMenuEntry(const char* key,const char* name,const char* page,bool isCallback=true){Key=key;Name=name;Page=page;IsCallbackPage=isCallback;}
};
#endif
