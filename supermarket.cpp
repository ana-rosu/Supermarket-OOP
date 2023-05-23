#include <typeinfo>
#include <iostream>
#include <fstream>
#include <conio.h>
#include <ctime>
#include <vector>
#include <set>
#include <map>
#include <string>
#include <algorithm>
using namespace std;

//this class publicly inherits from the std::exception class => FileNotFound becomes
// a specific type of exception that can be caught and handled using the standard exception handling mechanism.
class FileNotFound : public exception {
public:
// this is a member function of the FileNotFound class
// it overrides the what() function defined in the base class std::exception
    virtual const char* what() const throw(){
        return "File not found!";
    }
};

class IOInterface{
public:
    virtual istream& read(istream&, bool) = 0;
    virtual ostream& print(ostream&) const = 0;
};

class Product: public IOInterface{
protected:
  string name;
  int quantity;
  double price;
public:
  Product();
  Product(string, int, double);
  Product(const Product&);
  Product& operator =(const Product&);
  string getName(){return this->name;}
  double getPrice(){return this->price;}
  int getQuantity(){return this->quantity;}
  void setQuantity(int quantity){this->quantity = quantity;}
  istream& read(istream&, bool);
  ostream& print(ostream&) const;
  friend istream& operator >>(istream&, Product&);
  friend ostream& operator <<(ostream&, const Product&);
  virtual bool isOnSale() const = 0;
  virtual ~Product(){};
};

class HouseholdProduct: virtual public Product{
protected:
  string material;
  bool hazardous;
public:
  HouseholdProduct();
  HouseholdProduct(string, int, double, string, bool);
  HouseholdProduct(const HouseholdProduct&);
  HouseholdProduct& operator=(const HouseholdProduct&);
  istream& read(istream&, bool);
  ostream& print(ostream&) const;
  friend istream& operator >>(istream&, HouseholdProduct&);
  friend ostream& operator <<(ostream&, const HouseholdProduct&);
  bool isOnSale() const;
  virtual ~HouseholdProduct(){}
};

class ElectricalProduct: virtual public Product{
protected:
  int warrantyYears;
  int powerConsumption;
public:
  ElectricalProduct();
  ElectricalProduct(string, int, double, int, int);
  ElectricalProduct(const ElectricalProduct&);
  ElectricalProduct& operator=(const ElectricalProduct&);
  istream& read(istream&, bool);
  ostream& print(ostream&) const;
  friend istream& operator >>(istream&, ElectricalProduct&);
  friend ostream& operator <<(ostream&, const ElectricalProduct&);
  bool isOnSale() const;
  virtual ~ElectricalProduct(){}
};

class ElectricalAppliance: public HouseholdProduct, public ElectricalProduct{
  //enum noiseLvl = {low, medium, high};
  int noiseLevel;
  vector<string> features;
   //self-cleaning, automatic shut-off, water resistance, wi-fi, bluetooth
public:
  ElectricalAppliance();
  ElectricalAppliance(string, int, double, string, bool, int, int, int, vector<string>);
  ElectricalAppliance(const ElectricalAppliance&);
  ElectricalAppliance& operator=(const ElectricalAppliance&);
  istream& read(istream&, bool);
  ostream& print(ostream&) const;
  friend istream& operator >>(istream&, ElectricalAppliance&);
  friend ostream& operator <<(ostream&, const ElectricalAppliance&);
  bool isOnSale() const;
  ~ElectricalAppliance(){}
};

class Food: public Product{
public:
  enum category{
    vegetables, fruits, diary, meat, bakery, snacks
  };
private:
  category foodCat;
  string expDate;
  int kcal;
  int grams;
  bool organic;
public:
  Food();
  Food(string, int, double, category, string, int, int, bool);
  Food(const Food&);
  Food& operator =(const Food&);
  istream& read(istream&, bool);
  ostream& print(ostream&) const;
  friend istream& operator >>(istream&, Food&);
  friend ostream& operator <<(ostream&, const Food&);
  bool isOnSale() const;
  ~Food(){}
};

class Supermarket{
  string name;
  int foundationYear;
  vector<Product*> products;
public:
  Supermarket();
  Supermarket(string, int);
  Supermarket(string, int, vector<Product*>);
  Supermarket(const Supermarket&);
  Supermarket& operator=(const Supermarket&);
  friend istream& operator>>(istream&, Supermarket&);
  friend ostream& operator<<(ostream&, const Supermarket&);
  const vector<Product*> getProducts() const{return this->products;}
// welcome message
  void welcome();
// CRUD operations
  void addProduct();
  void showProducts();
  void editProduct();
  void delProduct();
  // functionality
  double calcRevenue(map<int, double>);
  //import data
  void importData();

  ~Supermarket();
};

class Cart{
  vector<Product*> cart;
public:
  Cart();
  Cart(vector<Product*>);
  Cart(const Cart&);
  Cart& operator=(const Cart&);
  friend istream& operator>>(istream&, Cart&);
  friend ostream& operator<<(ostream&, const Cart&);
  void add(Supermarket&);
  void del(Supermarket&);
  double buy();
  ~Cart();
};

class App {
private:
    static App* instance;
    static int nrOfInstances;
    map<int, double> sales;
    int cnt;
    Supermarket s;
    Cart c;
    App():cnt(1){
        s.importData();
        system("pause");
    };
    App(const App&)=delete;
    App& operator=(const App&) = delete;
public:
    static App* getInstance(){
        nrOfInstances++;
        if(!instance)
            instance=new App();
        return instance;
    }
    void run();
 ~App(){
    nrOfInstances--;
    if(nrOfInstances==0)
        if(instance)
            delete instance;

}
};
App* App::instance = nullptr;
int App::nrOfInstances = 0;

////////////////////////////////////////

Product::Product(){
  this->name = "anonymous";
  this->quantity = 0;
  this->price = 0;
}
Product::Product(string name, int quantity, double price){
  this->name = name;
  this->quantity = quantity;
  this->price = price;
}
Product::Product(const Product& p){
  this->name = p.name;
  this->quantity = p.quantity;
  this->price = p.price;
}
Product& Product::operator = (const Product& p){
  if(this != &p){
    this->name = p.name;
    this->quantity = p.quantity;
    this->price = p.price;
  }
  return *this;
}
istream& Product::read(istream& in, bool fromFile = 0){
  if(!fromFile){
    cout<<"Enter the name of the product: ";
    getline(in>>ws,this->name);

    cout << "Enter the quantity: ";
    int q;
    while (true) {
        try {
            if (!(in >> q))
                throw runtime_error("Invalid input. Please enter an integer: ");
            if (q < 0) {
                throw runtime_error("Invalid input. Quantity cannot be negative.\nPlease try again:");
            }
            this->quantity = q;
            break;
        } catch (const exception& e) {
            cout << e.what();
            // when an input operation fails (such as when the user enters invalid data),
            // the error flag is set, which can prevent further input operations from working correctly.
            // => reset the error flags, allowing the input stream to be used again
            in.clear();
            // discard the remaining characters in the input stream up to AND including the newline character
            in.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
  }
  else {
    getline(in>>ws,this->name);
    in>>this->quantity;
    in>>this->price;
  }
  return in;
}

ostream& Product::print(ostream& out) const{
  out<<"Name: "<<this->name<<endl;
  out<<"Quantity: "<<this->quantity<<" buc"<<endl;
  out<<"Price: "<<this->price<<" $"<<endl;
  return out;
}
istream& operator>>(istream& in, Product& p){
  return p.read(in);
}
ostream& operator<<(ostream& out, const Product& p) {
  return p.print(out);
}

////////////////////////////////////////

HouseholdProduct::HouseholdProduct():Product(){
  this->material = "unknown";
  this->hazardous = false;
}
HouseholdProduct::HouseholdProduct(string name, int quantity, double price, string material, bool hazardous):Product(name, quantity, price){
  this->material = material;
  this->hazardous = hazardous;
}
HouseholdProduct::HouseholdProduct(const HouseholdProduct& h):Product(h){
  this->material = h.material;
  this->hazardous = h.hazardous;
}
HouseholdProduct& HouseholdProduct::operator=(const HouseholdProduct& h){
  if(this != &h){
    Product::operator=(h);
    this->material = h.material;
    this->hazardous = h.hazardous;
  }
  return *this;
}
istream& HouseholdProduct::read(istream& in, bool fromFile = 0){
  if(!fromFile){
      Product::read(in);
      cout<<"Enter material: ";
      in>>this->material;
      cout<<"Is hazardous? [0/1] ";
      in>>this->hazardous;
  }
  else {
    Product::read(in, 1);
    in>>this->material;
    in>>this->hazardous;
  }
  return in;
}
ostream& HouseholdProduct::print(ostream& out) const {
  Product::print(out);
  out<<"Material: "<<this->material<<endl;
  out<<"Hazardous: "<<(this->hazardous == true ? "Yes" : "No")<<endl;
  return out;
}
istream& operator>>(istream& in, HouseholdProduct& h){
  return h.read(in);
}
ostream& operator<<(ostream& out, const HouseholdProduct& h){
  return h.print(out);
}
//to do: implement some logic here
bool HouseholdProduct::isOnSale() const{
  return 1;
}

////////////////////////////////////////

ElectricalProduct::ElectricalProduct():Product(){
  this->warrantyYears = 0;
  this->powerConsumption = 0;
}
ElectricalProduct::ElectricalProduct(string name, int quantity, double price, int warrantyYears, int powerConsumption):Product(name, quantity, price){
  this->warrantyYears = warrantyYears;
  this->powerConsumption = powerConsumption;
}
ElectricalProduct::ElectricalProduct(const ElectricalProduct& ep):Product(ep){
  this->warrantyYears = ep.warrantyYears;
  this->powerConsumption = ep.powerConsumption;
}
ElectricalProduct& ElectricalProduct::operator=(const ElectricalProduct& ep){
  if(this != &ep){
    Product::operator=(ep);
    this->warrantyYears = ep.warrantyYears;
    this->powerConsumption = ep.powerConsumption;
  }
  return *this;
}
istream& ElectricalProduct::read(istream& in, bool fromFile = 0){
  if(!fromFile){
      Product::read(in);
      cout<<"Enter warranty years: ";
      in>>this->warrantyYears;
      cout<<"Enter power consumption ";
      in>>this->powerConsumption;
  }
  else {
    Product::read(in,1);
    in>>this->warrantyYears;
    in>>this->powerConsumption;
  }
  return in;
}
ostream& ElectricalProduct::print(ostream& out) const {
  Product::print(out);
  out<<"Warranty years: "<<this->warrantyYears<<" yrs"<<endl;
  out<<"Power consumption: "<<this->powerConsumption<<" watts"<<endl;
  return out;
}
istream& operator>>(istream& in, ElectricalProduct& ep){
  return ep.read(in);
}
ostream& operator<<(ostream& out, const ElectricalProduct& ep){
  return ep.print(out);
}
bool ElectricalProduct::isOnSale() const{
  return 0;
}

////////////////////////////////////////

ElectricalAppliance::ElectricalAppliance(){
  this->noiseLevel = 0;
  this->features = {};
}
ElectricalAppliance::ElectricalAppliance(string name, int quantity, double price, string material, bool hazardous, int warrantyYears, int powerConsumption, int noiseLevel, vector<string> features):Product(name, quantity, price), HouseholdProduct(name, quantity, price,material, hazardous), ElectricalProduct(name, quantity, price, warrantyYears, powerConsumption){
  this->noiseLevel = noiseLevel;
  this->features = features;
}
ElectricalAppliance::ElectricalAppliance(const ElectricalAppliance& ea):Product(ea), HouseholdProduct(ea), ElectricalProduct(ea){
  this->noiseLevel = ea.noiseLevel;
  this->features = ea.features;
}
ElectricalAppliance& ElectricalAppliance::operator=(const ElectricalAppliance& ea){
  if(this != &ea){
    HouseholdProduct::operator=(ea);
    ElectricalProduct::operator=(ea);
    this->noiseLevel = ea.noiseLevel;
    this->features = ea.features;
  }
  return *this;
}
istream& ElectricalAppliance::read(istream& in, bool fromFile = 0){
  if(!fromFile){
      HouseholdProduct::read(in);
      cout<<"Enter warranty years: ";
      in>>this->warrantyYears;
      cout<<"Enter power consumption ";
      in>>this->powerConsumption;
      cout<<"Enter noise level: ";
      in>>this->noiseLevel;
      cout<<"Enter features: \n";
      if(!this->features.empty())
        this->features.clear();
      while(true){
        cout<<"1. Add feature\n0. Stop\n";
        int choice;
        in>>choice;
        if(choice == 0)
          break;
        else{
          string feature;
          getline(in>>ws,feature);
          this->features.push_back(feature);}
      }
  } else {
      HouseholdProduct::read(in,1);
      in>>this->warrantyYears;
      in>>this->powerConsumption;
      in>>this->noiseLevel;
      if(!this->features.empty())
        this->features.clear();
      while(true){
        int choice;
        in>>choice;
        if(choice == 0)
          break;
        else{
          string feature;
          getline(in>>ws,feature);
          this->features.push_back(feature);}
      }
  }
  return in;
}
ostream& ElectricalAppliance::print(ostream& out) const {
  HouseholdProduct::print(out);
  out<<"Warranty years: "<<this->warrantyYears<<" yrs"<<endl;
  out<<"Power consumption: "<<this->powerConsumption<<" watts"<<endl;
  out<<"Noise level: "<<this->noiseLevel<<endl;
  out<<"Features: ";
  for(int i = 0; i < this->features.size()-1; ++i)
    out<<this->features[i]<<", ";
  out<<this->features[this->features.size()-1]<<endl;
  return out;
}
istream& operator>>(istream& in, ElectricalAppliance& ea){
  return ea.read(in);
}
ostream& operator<<(ostream& out, const ElectricalAppliance& ea){
  return ea.print(out);
}
//to do: implement some logic here
bool ElectricalAppliance::isOnSale() const{
  return 0;
}

////////////////////////////////////////

Food::Food():Product(){
  this->foodCat = vegetables;
  this->expDate = "N/A";
  this->kcal = 0;
  this->grams = 0;
  this->organic = false;
}
Food::Food(string name, int quantity, double price, category foodCat, string expDate, int kcal, int grams, bool organic):Product(name, quantity, price){
  this->foodCat = foodCat;
  this->expDate = expDate;
  this->kcal = kcal;
  this->grams = grams;
  this->organic = organic;
}
Food::Food(const Food& f):Product(f){
  this->foodCat = f.foodCat;
  this->expDate = f.expDate;
  this->kcal = f.kcal;
  this->grams = f.grams;
  this->organic = f.organic;
}
Food& Food::operator=(const Food& f){
  if(this != &f){
    Product::operator=(f);
    this->foodCat = f.foodCat;
    this->expDate = f.expDate;
    this->kcal = f.kcal;
    this->grams = f.grams;
    this->organic = f.organic;
  }
  return *this;
}
istream& Food::read(istream& in, bool fromFile = 0){
  if(!fromFile){
      Product::read(in);
      cout<<"Enter the category of the product:\n[0: vegetables/ 1: fruits/ 2: diary/ 3: meat/ 4: bakery/ 5: snacks] ";
      int cat;
      while(true){
        try{
            in>>cat;
            if (cat < 0 || cat > 5)
                throw runtime_error("Invalid category. Please enter a number in the range 0-5: ");
            else break;
        }catch(runtime_error& e){
            cout<<e.what();
        }
      }
      this->foodCat = static_cast<Food::category>(cat);
      cout<<"Enter the expiration date: ";
      in>>this->expDate;
      cout<<"Enter kcal/100g: ";
      in>>this->kcal;
      cout<<"Enter the weight in grams: ";
      in>>this->grams;
      cout<<"Is organic? [0/1] ";
      in>>this->organic;
  }
  else {
      Product::read(in,1);
      int cat;
      in>>cat;
      this->foodCat = static_cast<Food::category>(cat);
      in>>this->expDate;
      in>>this->kcal;
      in>>this->grams;
      in>>this->organic;
  }
  return in;
}
ostream& Food::print(ostream& out) const{
  Product::print(out);
  vector<string> categories = {"vegetables", "fruits", "diary", "meat", "bakery", "snacks"};
  out<<"Category: "<<categories[this->foodCat]<<endl;
  out<<"Expiration date: "<<this->expDate<<endl;
  out<<"Kcal: "<<this->kcal<<" kcal/100g"<<endl;
  out<<"Weight: "<<this->grams<<"g"<<endl;
  out<<"Organic: "<<(this->organic ? "Yes" : "No")<<endl;
  return out;
}
istream& operator>>(istream& in, Food& f){
  return f.read(in);
}
ostream& operator<<(ostream& out, const Food& f){
  return f.print(out);
}
//to do: based on expiration date
bool Food::isOnSale() const{
  if(this->organic == true)
    return 0;
  return 1;
}

//////////////////////////////////////////

Cart::Cart(){
  this->cart = {};
}
Cart::Cart(vector<Product*> cart){
  this->cart = cart;
}
Cart::Cart(const Cart& c){
  this->cart = c.cart;
}
Cart& Cart::operator=(const Cart& c){
  if(this!=&c){
    this->cart = c.cart;
  }
  return *this;
}

void Cart::add(Supermarket& s){
  string name;
  int quantity;
  cout<<"Enter the name: ";
  getline(cin>>ws,name);
  cout<<"Enter the quantity: ";
  cin>>quantity;
  for(int i = 0; i < s.getProducts().size(); ++i)
    if(s.getProducts()[i]->getName() == name){
      int q = s.getProducts()[i]->getQuantity();
      if(q > 0){
        //update the quantity of the product in the supermarket
        q-=quantity;
        s.getProducts()[i]->setQuantity(q);

        //making a copy of the product chosen to buy, in order to add it to the cart with the correct quantity
        Product* p;

        if(typeid(*s.getProducts()[i]) == typeid(Food))
           p = new Food(dynamic_cast<Food&>(*s.getProducts()[i]));
        if(typeid(*s.getProducts()[i]) == typeid(HouseholdProduct))
           p = new HouseholdProduct(dynamic_cast<HouseholdProduct&>(*s.getProducts()[i]));
        if(typeid(*s.getProducts()[i]) == typeid(ElectricalProduct))
           p = new ElectricalProduct(dynamic_cast<ElectricalProduct&>(*s.getProducts()[i]));
        if(typeid(*s.getProducts()[i]) == typeid(ElectricalAppliance))
           p = new ElectricalAppliance(dynamic_cast<ElectricalAppliance&>(*s.getProducts()[i]));

        p->setQuantity(quantity);
        this->cart.push_back(p);
        }
    }
}

double Cart::buy(){
  ofstream receipt("receipt.out");

  try{
  //logic error -> cannot perform operations on an empty cart
    if (this->cart.size() == 0) {
        throw logic_error("The cart is empty. Cannot perform the order.");
    }
  }
  catch(logic_error& e){
    cout<<e.what();
    return -1;
  }

  double sum = 0;
  for(int i = 0; i < this->cart.size(); ++i)
    sum+=(this->cart[i]->getPrice()*this->cart[i]->getQuantity());

  time_t tim = time(0);
  tm* gottime =  localtime(&tim);
  receipt<<"Tranzaction time: "<<gottime->tm_mday<<".0"<<gottime->tm_mon+1<<"."<<gottime->tm_year+1900<<" "<<gottime->tm_hour<<":"<<gottime->tm_min<<endl;

  receipt<<"\n------List of goods------\n\n";
  for(int i = 0; i < this->cart.size(); ++i){
      receipt<<*(this->cart[i])<<endl;
  }

  receipt<<"\nTotal price: "<<sum<<"$";
  return sum;
}

void Cart::del(Supermarket& s){
  cout<<"What product do you want to delete? Enter the name: ";
  string name;
  getline(cin>>ws, name);
  vector<Product*> updatedCart;

  for(int i=0; i<cart.size();i++)
    if(cart[i]->getName() != name)
      updatedCart.push_back(cart[i]);
    else {
      // if the client has only one item of that product, simply update the stock of that specific product in the supermarket
      if(cart[i]->getQuantity() == 1)
        for(int i = 0; i < s.getProducts().size(); ++i)
          if(s.getProducts()[i]->getName() == name)
            s.getProducts()[i]->setQuantity(s.getProducts()[i]->getQuantity()+1);

      // if the client has more items of that product, ask him how many does he want to delete, if he chooses
      // to delete all of them, then delete the product entirely from the cart and update the stock in the market, else update the quantity of the product in the cart and also update the stock in the market
      if(cart[i]->getQuantity() != 1){
        cout<<"How many items of this product do you want to delete? ";
        int nr, newQuantity;
        cin>>nr;

        //input validation
        try{
          if(nr > cart[i]->getQuantity() || nr < 0)
            throw runtime_error("Invalid quantity!\n");
        }
        catch(const runtime_error& e) {
            cout<<e.what();
            while(nr > cart[i]->getQuantity() || nr < 0){
              cout<<"\nEnter a number less or equal to the quantity in your cart!! ";
              cin>>nr;
            }
        }

        //update the stock in the supermarket
        for(int i = 0; i < s.getProducts().size(); ++i)
          if(s.getProducts()[i]->getName() == name)
            s.getProducts()[i]->setQuantity(s.getProducts()[i]->getQuantity()+nr);

        //update the quantity of the product in the cart
        newQuantity = cart[i]->getQuantity() - nr;
        if(newQuantity > 0){
          //product is an abstract class so I can't make a copy by instantiating Product* p = new Product(*cart[i]);
          Product* p;

          if(typeid(*cart[i]) == typeid(Food))
            p = new Food(dynamic_cast<Food&>(*cart[i]));
          if(typeid(*cart[i]) == typeid(HouseholdProduct))
            p = new HouseholdProduct(dynamic_cast<HouseholdProduct&>(*cart[i]));
          if(typeid(*cart[i]) == typeid(ElectricalProduct))
            p = new ElectricalProduct(dynamic_cast<ElectricalProduct&>(*cart[i]));
          if(typeid(*cart[i]) == typeid(ElectricalAppliance))
            p = new ElectricalAppliance(dynamic_cast<ElectricalAppliance&>(*cart[i]));

          p->setQuantity(newQuantity);
          updatedCart.push_back(p);
        }
      }
    }
  cart = updatedCart;
  for(auto product : updatedCart)
    delete product;
  updatedCart.clear();
  cout<<"\nProduct deleted succesfully!\n\n";
}
Cart::~Cart(){
    for(auto productPtr : cart)
      delete productPtr;
    cart.clear();
}

/////////////////////////////////////////

Supermarket::Supermarket(){
  this->name = "Unknown";
  this->foundationYear = 0;
  this->products = {};
}
Supermarket::Supermarket(string name, int foundationYear){
  this->name = name;
  this->foundationYear = foundationYear;
  this->products = {};
}
Supermarket::Supermarket(string name, int foundationYear, vector<Product*> products){
  this->name = name;
  this->foundationYear = foundationYear;
  this->products = products;
}
Supermarket::Supermarket(const Supermarket& s){
  this->name = s.name;
  this->foundationYear = s.foundationYear;
  this->products = s.products;
}
Supermarket& Supermarket::operator=(const Supermarket& s){
  if(this != &s){
    this->name = s.name;
    this->foundationYear = s.foundationYear;
    this->products = s.products;
  }
  return *this;
}
istream& operator>>(istream& in, Supermarket& s){
  cout<<"Enter the name of the supermarket: ";
  in>>s.name;
  cout<<"Enter the year of the foundation: ";
  in>>s.foundationYear;
  cout<<"How many products do you want to add: ";
  int nr;
  in>>nr;
  if(nr){
    cout<<"Enter the products of the supermarket: \n\n";
    if(!s.products.empty())
      s.products.clear();
    while(nr){
      s.addProduct();
      nr--;
    }
  }
  return in;
}
ostream& operator<<(ostream& out, const Supermarket& s){
  out<<"Name of the supermarket: "<<s.name<<endl;
  out<<"Year of foundation: "<<s.foundationYear<<endl;
  out<<"Products: ";
  if(s.products.empty())
    out<<"0";
  else{
    out<<"\n\n";
    for(int i = 0; i < s.products.size(); ++i)
      out<<*s.products[i]<<endl;
  }
  return out;
}
void Supermarket::welcome(){
//  system("cls");
  cout<<"\n\t Welcome to "<<this->name<<" supermarket! Shopping made easy since "<<this->foundationYear<<" :)\n\n";
}
void Supermarket::importData(){
    try {
        cout<<"Please provide the name of the file from which to import data:  ";
        string file;
        cin>>file;
        ifstream fin(file);

        if(!fin)
            throw FileNotFound();
        else cout<<"\nData was imported succesfully!!\n\n";

        fin>>this->name;
        fin>>this->foundationYear;

        string productType;
        while(fin>>productType){
            if(productType == "FOOD"){
                // this line dynamically allocates memory for a Food object on the heap and assigns its address to the pointer f.
                // now f points to a dynamically created Food object.
                Food* f = new Food();
                // reads input from the file and stores it in the Food object pointed to by f
                // operator >> is overloaded
                (*f).read(fin, 1);

                //adds the pointer f to the listOfProducts vector
                this->products.push_back(f);
                }
            if(productType == "HOUSEHOLD"){
                HouseholdProduct* hp = new HouseholdProduct();
                (*hp).read(fin, 1);
                this->products.push_back(hp);
                }
            if(productType == "ELECTRICAL"){
                ElectricalProduct* e = new ElectricalProduct();
                (*e).read(fin, 1);
                this->products.push_back(e);
                }
            if(productType == "ELECTRICAL-APPLIANCE"){
                ElectricalAppliance* ea = new ElectricalAppliance();
                (*ea).read(fin, 1);
                this->products.push_back(ea);
            }
        }
        fin.close();
    }
    catch(FileNotFound& e){
        cout<<e.what()<<endl;
    }
}
void Supermarket::addProduct(){
  cout<<"What type of product do you want to add?\n";
  cout<<"\t 1. Food\n\t 2. Household item\n\t 3. Electrical device\n\t 4. Electrical appliance\n";
  int choice;
  Product* p;
  while(true){
      try{
          cin>>choice;
          switch (choice){
            case 1:{
              p = new Food();
              break;
            }
            case 2:{
              p = new HouseholdProduct();
              break;
            }
            case 3:{
              p = new ElectricalProduct();
              break;
            }
            case 4:{
              p = new ElectricalAppliance();
              break;
            }
            default:{
              throw runtime_error("Invalid choice. Please enter a number in the range 1-4.");
            }
          }
          //break the loop if input is valid
          break;
      } catch (const exception& e) {
                cout << e.what() << " Please try again: ";
      }
  }
  cin>>*p;
  products.push_back(p);
  cout<<"\nProduct added succesfully!\n\n";
}
void Supermarket::editProduct(){
  cout<<"What product do you want to edit? Enter the name: ";
  string name;
  cin>>name;
  for(int i=0; i<products.size();i++)
    if(products[i]->getName() == name)
      cin>>*products[i];
  cout<<"\nProduct edited succesfully!\n\n";
}
void Supermarket::showProducts(){
  cout<<"\t Here's a list of all the products of our supermarket!\n\n";
  cout<<"--------------------------------\n";
  for(int i = 0; i < products.size(); ++i){
    cout<<*products[i];
    cout<<"--------------------------------\n";
  }
  cout<<endl;
}
void Supermarket::delProduct(){
  cout<<"What product do you want to delete? Enter the name: ";
  string name;
  cin>>name;
  vector<Product*> updatedProducts;
  for(int i=0; i<products.size();i++)
    if(products[i]->getName() != name)
      updatedProducts.push_back(products[i]);

  products = updatedProducts;
  for(auto pr : updatedProducts)
    delete pr;
  cout<<"\nProduct deleted succesfully!\n\n";
}
double Supermarket::calcRevenue(map<int, double> sales){
  double rev = 0;
  map<int, double>::iterator it;
  for(auto it = sales.begin(); it != sales.end(); it++)
    rev+=it->second;
  return rev;
}
Supermarket::~Supermarket(){
    for(auto productPtr : products)
      delete productPtr;
    products.clear();
}

////////////////////////////////////////

// Template functions
// Sort products by ascending price
bool compareByPrice(Product* p1, Product* p2) {
    return p1->getPrice() < p2->getPrice();
}
// Sort products by alphabetical order
bool compareByName(Product* p1, Product* p2) {
    int res = p1->getName().compare(p2->getName());
    return res < 0;
}
template <typename T>
vector<Product*> sortProducts(vector<Product*> products, T func) {
  vector<Product*> sortedProducts;
  for (const auto& product : products) {
    Product* p;
    if(typeid(*product) == typeid(Food))
      p = new Food(dynamic_cast<Food&>(*product));
    if(typeid(*product) == typeid(HouseholdProduct))
      p = new HouseholdProduct(dynamic_cast<HouseholdProduct&>(*product));
    if(typeid(*product) == typeid(ElectricalProduct))
      p = new ElectricalProduct(dynamic_cast<ElectricalProduct&>(*product));
    if(typeid(*product) == typeid(ElectricalAppliance))
      p = new ElectricalAppliance(dynamic_cast<ElectricalAppliance&>(*product));
    sortedProducts.push_back(p);
  }
  sort(sortedProducts.begin(), sortedProducts.end(), func);
  return sortedProducts;
}
// Filter products
bool isProductExpensive(Product* product) {
    return product->getPrice() > 100.0;
}
bool isProductFood(Product* product){
  if(dynamic_cast<Food*>(product))
    return true;
  return false;
}
bool isProductHousehold(Product* product){
  if(dynamic_cast<HouseholdProduct*>(product))
    return true;
  return false;
}
bool isProductElectrical(Product* product){
  if(dynamic_cast<ElectricalProduct*>(product))
    return true;
  return false;
}
bool isProductElectricalAppliance(Product* product){
  if(dynamic_cast<ElectricalAppliance*>(product))
    return true;
  return false;
}

template <typename T>
set<Product*> filterProducts(vector<Product*> products, T func) {
    set<Product*> filteredProducts;
    for (const auto& product : products) {
        if (func(product)) {
          Product* p;
        if(typeid(*product) == typeid(Food))
          p = new Food(dynamic_cast<Food&>(*product));
        if(typeid(*product) == typeid(HouseholdProduct))
          p = new HouseholdProduct(dynamic_cast<HouseholdProduct&>(*product));
        if(typeid(*product) == typeid(ElectricalProduct))
          p = new ElectricalProduct(dynamic_cast<ElectricalProduct&>(*product));
        if(typeid(*product) == typeid(ElectricalAppliance))
          p = new ElectricalAppliance(dynamic_cast<ElectricalAppliance&>(*product));
          filteredProducts.insert(p);
        }
    }
    return filteredProducts;
}
template<typename Container>
void printProducts(Container& products) {
    cout << "--------------------------------\n";
    for (const auto& productPtr : products) {
        cout << *productPtr;
        cout << "--------------------------------\n";
    }
    cout << endl;
}
template<typename Container>
void clearProducts(Container& products){
    for(const auto& product : products)
        delete product;

    products.clear();
}

/////////////////////////////////////////

void App::run(){
  int k = 0;
  while(k!=-1){
      system("cls");
      s.welcome();
      cout<<"1. Admin\n2. Client\n3. Exit\n\n";
      cin>>k;
      cout<<endl;
      if(k == 1){
        while(k == 1){
            cout << "1. Add product\n";
            cout << "2. Show products\n";
            cout << "3. Edit product\n";
            cout << "4. Delete product\n\n";
            cout << "5. Supermarket's revenue\n";
            cout << "6. Back to menu\n\n";
            int choice;
            cin >> choice;
            switch(choice){
                case 1:{
                    s.addProduct();
                    break;
                }
                case 2:{
                    s.showProducts();
                    break;
                }
                case 3:{
                    s.editProduct();
                    break;
                }
                case 4:{
                    s.delProduct();
                    break;
                }
                case 5:{
                    cout<<"\nCurrent revenue: "<<s.calcRevenue(sales)<<"$\n\n";
                    break;
                }
                case 6:{
                    k = 0;
                    break;
                }
            }
        }
      }
      else if (k == 2){
            s.showProducts();
        while(k == 2){

            cout << "1. Sort products by price\n";
            cout << "2. Sort products by name\n";
            cout << "3. Show only expensive products\n";
            cout << "4. Filter products by category\n\n";
            cout << "5. Add product to the cart\n";
            cout << "6. Delete product from the cart\n";
            cout << "7. Place order\n";
            cout << "8. Back to menu\n\n";
            int choice;
            cin >> choice;
            switch(choice){
                case 1:{
                    vector<Product*> a = sortProducts(s.getProducts(), compareByPrice);
                    printProducts(a);
                    clearProducts(a);
                    break;
                }
                case 2:{
                    vector<Product*> b = sortProducts(s.getProducts(), compareByName);
                    printProducts(b);
                    clearProducts(b);
                    break;
                }
                case 3:{
                    set<Product*> onlyExpensive = filterProducts(s.getProducts(), isProductExpensive);
                    printProducts(onlyExpensive);
                    clearProducts(onlyExpensive);
                    break;
                }
                case 4:{
                    cout<<"Enter the category [1-Food, 2-Household Products, 3-Electrical Products, 4-Electrical Appliance]: ";
                    int c;
                    cin>>c;
                    switch(c){
                        case 1:{
                            set<Product*> onlyFood = filterProducts(s.getProducts(), isProductFood);
                            printProducts(onlyFood);
                            clearProducts(onlyFood);
                            break;
                        }
                        case 2:{
                             set<Product*> onlyHousehold = filterProducts(s.getProducts(), isProductHousehold);
                             printProducts(onlyHousehold);
                             clearProducts(onlyHousehold);
                            break;
                        }
                        case 3:{
                             set<Product*> onlyElectrical = filterProducts(s.getProducts(), isProductElectrical);
                             printProducts(onlyElectrical);
                             clearProducts(onlyElectrical);
                            break;
                        }
                        case 4:{
                             set<Product*> onlyElectricalAppliance = filterProducts(s.getProducts(), isProductElectricalAppliance);
                             printProducts(onlyElectricalAppliance);
                             clearProducts(onlyElectricalAppliance);
                            break;
                        }
                        default:{
                            cout<<"\nInvalid option!\n";
                            break;
                        }
                    }
                    break;
                }
                case 5:{
                    c.add(s);
                    break;
                }
                case 6:{
                    c.del(s);
                    break;
                }
                case 7:{
                    double sale = c.buy();
                    //if the cart is not empty, issue a receipt
                    if(sale!=-1){
                        sales.insert(make_pair(cnt++, sale));
                        //if in the same session, multiple orders are made, initialize an empty cart for the next order
                        c = Cart();
                    }
                    break;
                }
                case 8:{
                 k = 0;
                 break;
                }
            }
        }
      }
      else if (k == 3) k = -1;
      else {
        cout<<"\t Invalid choice!";
        k = -1;
      }
  }
}

/////////////////////////////////////////////////

int main(){

  App* app = app->getInstance();
  app->run();
  return 0;
}
//class IOInterface;
//class Supermarket;
//class Cart;
//class Product;
//class HouseholdProduct;
//class ElectricalProduct;
//class ElectricalAppliance;
//class Food;
//class FileNotFound;
//class App;
//to do
//discount manager template class
//use list










