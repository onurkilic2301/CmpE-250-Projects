#include "SurveyClass.h"
#include <map>
SurveyClass::SurveyClass(){
    members = new LinkedList();
};
SurveyClass::SurveyClass(const SurveyClass &other){
    if(other.members)
        members = new LinkedList(*(other.members));
};
SurveyClass& SurveyClass::operator=(const SurveyClass& list){
    if(members)
        delete members;
    this->members = new LinkedList(*(list.members));

};
SurveyClass::~SurveyClass(){
    delete members;
};

// Adds a new Member object to the linked list
void SurveyClass::addMember(const Member& newMember){
    members->pushTail(newMember);

};
// Calculates and returns the average age for the members of the class.
// The average age can have up to two decimal points.
// If there is no member returns 0
float SurveyClass::calculateAverageAge(){
    if(members->head) {
        float sumOfAges = 0;
        Node *temp = members->head;
        //Iterating to sum age of members
        for (int i = 0; i < members->length; i++) {
            sumOfAges += temp->data.age;
            if (temp->next)
                temp = temp->next;
        }
        float avgAge = ((int) (sumOfAges / members->length * 100 + 0.5) / 100.0);
        return avgAge;
    }
    else
        return 0;

};
// Finds the most favourite color and returns its name.
// The most favourite color is the color
// which is liked by the highest number of members.
// If there is no member empty string is returned
string SurveyClass::findMostFavouriteColor(){
    if(members->head){
        //Information of colors and how many times are they used is stored in mapOfColors
        std::map<string,int> mapOfColors;
        Node *temp = members->head;
        for(int i = 0; i < members->length; i++){
            string tmpColor = temp->data.color;
            if(mapOfColors[tmpColor])
                mapOfColors[tmpColor]+=1;
            else
                mapOfColors[tmpColor] = 1;
            temp = temp->next;
        }
        int max = 0;
        string word;
        //Iterating mapOfColors to find the color that is liked by the members most
        for (std::map<string,int>::iterator it=mapOfColors.begin(); it!=mapOfColors.end(); ++it){
            if(it->second>max){
                max=it->second;
                word=it->first;
            }
        }
        return word;
    }
    else
        return "";
};