#include <iostream>
#include <fstream>
#include "contacts.pb.h" 
using namespace std; 
using namespace contacts;

/*
*打印联系人列表
*/
void PrintfContacts(const Contacts& contacts){
    for (int i = 0; i < contacts.contacts_size(); ++i) {
        const PeopleInfo& people = contacts.contacts(i);
        cout << "------------联系⼈" << i+1 << "------------" << endl;
        cout << "姓名：" << people.name() << endl;
        cout << "年龄：" << people.age() << endl;
        int j = 1;
        for (const PeopleInfo_Phone& phone : people.phone()) {
            cout<<"电话"<<j++<<": "<<phone.number();
            cout<<"("<<phone.PhoneType_Name(phone.type()) <<")" <<endl;
        }

        if(people.has_data()&&people.data().Is<Address>()){
            Address address;
            people.data().UnpackTo(&address);
            if(!address.home_address().empty()){
                cout<< "家庭地址："<<address.home_address()<<endl;
            }
            if(!address.unit_address().empty()){
                cout<<"单位地址："<<address.unit_address() <<endl;
            }
        }

        /*if (people.has_qq()){

        }else if(people.has_weixin()){

        }
        */
        switch(people.other_contact_case()){
            case PeopleInfo::OtherContactCase::kQq:
                cout<<"qq号："<<people.qq()<<endl;
                break;
            case PeopleInfo::OtherContactCase::kWeixin:
                cout<<"微信号："<<people.weixin()<<endl;
                break;
            case PeopleInfo::OtherContactCase::OTHER_CONTACT_NOT_SET:
                break;
                
        }
        if (people.remark_size()){
            cout<<"备注信息："<<endl;
        }
        for(auto it = people.remark().cbegin();it!=people.remark().cend();++it){
            cout<<" "<<it->first<<": "<<it->second<<endl;
        }
    }
}

int main(int argc,char* argv[])
{
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    if(argc != 2){
        cerr<<"Usage: "<<argv[0] <<"CONTACTS_FILE" <<endl;
        return -1;
    }

    //以二进制方式读取contacts
    Contacts contacts;
    fstream input(argv[1],ios::in | ios::binary);
    if (!contacts.ParseFromIstream(&input)) {
        cerr << "Failed to parse contacts." << endl;
        input.close();
        return -1;
    }

    PrintfContacts(contacts);

    input.close();
    google::protobuf::ShutdownProtobufLibrary();
    return 0;
}