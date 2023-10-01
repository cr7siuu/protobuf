#include <iostream>
#include <fstream>
#include "contacts.pb.h"
using namespace std;
using namespace contacts;

/*
新增联系人
*/
void AddPeopleInfo(PeopleInfo *people_info_ptr)
{
    cout<<"-----新增联系人-----" <<endl;
    cout<<"请输入联系人姓名： ";
    string name;
    getline(cin,name);
    people_info_ptr->set_name(name);

    cout << "请输⼊联系⼈年龄: ";
    int age;
    cin >> age;
    people_info_ptr->set_age(age);
    cin.ignore(256, '\n');

    for(int i = 1;;i++){
        cout<<"请输入联系人电话"<<i<<"(只输入回车完成电话新增)";
        string number;
        getline(cin,number);
        if(number.empty()){
            break;
        }
        PeopleInfo_Phone* phone = people_info_ptr->add_phone();
        phone->set_number(number);

        cout << "选择此电话类型（1、移动电话  2、固定电话）: ";
        int type;
        cin >> type;
        cin.ignore(256,'\n');
        switch(type){
            case 1:
                phone->set_type(PeopleInfo_Phone_PhoneType::PeopleInfo_Phone_PhoneType_MP);
                break;
            case 2:
                phone->set_type(PeopleInfo_Phone_PhoneType::PeopleInfo_Phone_PhoneType_TEL);
                break;
            defalut:
                cout<<"非法选择，使用默认值！"<<endl;
                break;
        }
    }

    Address address;
    cout<<"请输入联系人家庭地址： ";
    string home_address;
    getline(cin,home_address);
    address.set_home_address(home_address);
    cout<<"请输入联系人单位地址: ";
    string unit_address;
    getline(cin,unit_address);
    address.set_unit_address(unit_address);
    google::protobuf::Any* data = people_info_ptr->mutable_data();
    data->PackFrom(address);

    cout<<"选择添加一个其他联系方式（1、qq号 2、微信号）：";
    int other_contact;
    cin >> other_contact;
    cin.ignore(256,'\n');
    if(1 == other_contact){
        cout<<"请输入qq号：";
        string qq;
        getline(cin,qq);
        people_info_ptr->set_qq(qq);
    }else if(2 == other_contact){
        cout<<"请输入微信号： ";
        string weixin;
        getline(cin,weixin);
        people_info_ptr->set_weixin(weixin);
    }else{
        cout<<"非法选择，该项设置失败！"<<endl;
    }

    for(int i = 1;;i++){
        cout<<"请输入备注："<<i<<"标题（只输入回车完成备注新增): ";
        string remark_key;
        getline(cin,remark_key);
        if(remark_key.empty()){
            break;
        }

        cout<<"请输入备注："<< i << "内容：";
        string remark_value;
        getline(cin,remark_value);
        people_info_ptr->mutable_remark()->insert({remark_key,remark_value});
    }
    cout<<"新增联系人成功"<<endl;

}

int main(int argc,char *argv[])
{
    GOOGLE_PROTOBUF_VERIFY_VERSION;
    if(argc != 2){
        cerr<<"Usage: "<<argv[0] <<" CONTACTS_FILE"<<endl;
        return -1;
    }

    Contacts contacts;
    // 先读取已存在的 contacts
    fstream input(argv[1],ios::in | ios::binary);
    if(!input){
        cout<<argv[1]<<":File not found. Creating a new file." << endl;
    }
    else if(!contacts.ParseFromIstream(&input)){
        cerr  << "Failed to parse contacts." << endl;
        input.close();
        return -1;
    }
    //新增一个联系人
    AddPeopleInfo(contacts.add_contacts());

    // 向磁盘⽂件写⼊新的 contacts
    fstream output(argv[1], ios::out | ios::trunc | ios::binary);
    if (!contacts.SerializeToOstream(&output)){
        cerr << "Failed to write contacts." << endl;
        input.close();
        output.close();
        return -1;
    }
    input.close();
    output.close();
    google::protobuf::ShutdownProtobufLibrary();
    return 0;
}