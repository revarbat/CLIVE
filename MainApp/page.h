#ifndef PAGE_H
#define PAGE_H


class CPage {
private:
    char* title;

public:
    CPage(const char* title);
    virtual ~CPage();

    const char* GetTitle() const { return title; }
    virtual void Init() { }
    virtual void Display() = 0;
    virtual void HandleKeys(short key) = 0;

};


extern CPage* current_page;




class CMenuPage : public CPage {
public:
    CMenuPage(const char* title);
    ~CMenuPage();

    void AddItem(CPage* item);
    void Init();
    void Display();
    void HandleKeys(short key);


private:
    short selected_item;
    short listsize;
    short itemcount;
    CPage* previous_page;
    CPage** itemlist;

};

extern CMenuPage* main_menu_page;


void UpdateDisplay();
void HandleKeys();


#endif

