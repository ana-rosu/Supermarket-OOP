#include <iostream>
#include <fstream>
#include <conio.h>
#include <ctime>
#include <vector>
using namespace std;

class IOInterface;
class Supermarket;
class Cart;
class Product;
class HouseholdProduct;
class ElectricalProduct;
class ElectricalAppliance;
class Food;


class IOInterface{
public:
    virtual istream& read(istream&) = 0;
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
  istream& read(istream&);
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
  istream& read(istream&);
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
  istream& read(istream&);
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
  istream& read(istream&);
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
  istream& read(istream&);
  ostream& print(ostream&) const;
  friend istream& operator >>(istream&, Food&);
  friend ostream& operator <<(ostream&, const Food&);
  bool isOnSale() const;
  ~Food(){}
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
  void del();
  void buy();
  ~Cart(){}
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
  vector<Product*> getProducts() const{return this->products;}
  void setProducts(vector<Product*> products) {
    if(!this->products.empty())
      this->products.clear();
    for(int i = 0; i < products.size(); ++i)
      this->products[i] = products[i];
  }
  ~Supermarket(){}
// welcome message
  void welcome();
// CRUD operations
  void addProduct();
  void showProducts();
  void editProduct();
  void delProduct();
  // functionality
  int calcRevenue(){
    //based on all orders placed
  }
  ~Supermarket(){}
};
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
istream& Product::read(istream& in){
  cout<<"Enter the name of the product: ";
  in>>this->name;
  cout<<"Enter the quantity: ";
  in>>this->quantity;
  cout<<"Enter the price: ";
  in>>this->price;
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
istream& HouseholdProduct::read(istream& in){
  Product::read(in);
  cout<<"Enter material: ";
  in>>this->material;
  cout<<"Is hazardous? [0/1] ";
  in>>this->hazardous;
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
istream& ElectricalProduct::read(istream& in){
  Product::read(in);
  cout<<"Enter warranty years: ";
  in>>this->warrantyYears;
  cout<<"Enter power consumption ";
  in>>this->powerConsumption;
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

ElectricalAppliance::ElectricalAppliance():Product(), HouseholdProduct(), ElectricalProduct(){
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
istream& ElectricalAppliance::read(istream& in){
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
    cin>>choice;
    if(choice == 0)
      break;
    else{    
      string feature;
      cin>>feature;
      this->features.push_back(feature);}
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
  out<<this->features[this->features.size()-1];
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
istream& Food::read(istream& in){
  Product::read(in);
  cout<<"Enter the category of the product:\n[0: vegetables/ 1: fruits/ 2: diary/ 3: meat/ 4: bakery/ 5: snacks] ";
  int cat;
  in>>cat;
  this->foodCat = static_cast<Food::category>(cat);
  cout<<"Enter the expiration date: ";
  in>>this->expDate;
  cout<<"Enter kcal/100g: ";
  in>>this->kcal;
  cout<<"Enter the weight in grams: ";
  in>>this->grams;
  cout<<"Is organic? [0/1] ";
  in>>this->organic;
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
  cout<<"Enter the name: ";
  string name;
  cin>>name;
  for(int i = 0; i < s.getProducts().size(); ++i)
    if(s.getProducts()[i]->getName() == name){
      int q = s.getProducts()[i]->getQuantity();
      if(q > 0){
        q--;
        this->cart.push_back(s.getProducts()[i]);
        s.getProducts()[i]->setQuantity(q);
      }
    }
}

void Cart::buy(){
  ofstream receipt("receipt.out");

  double sum = 0;
  for(int i = 0; i < this->cart.size(); ++i)
    sum+=this->cart[i]->getPrice();

  time_t tim = time(0);
  tm* gottime =  localtime(&tim);
  receipt<<"Tranzaction time: "<<gottime->tm_mday<<".0"<<gottime->tm_mon+1<<"."<<gottime->tm_year+1900<<" "<<gottime->tm_hour<<":"<<gottime->tm_min<<endl;

  receipt<<"------List of goods------\n";
  for(int i = 0; i < this->cart.size(); ++i)
    receipt<<*this->cart[i]<<endl;
  receipt<<"\nTotal price: "<<sum;
}

void Cart::del(){
  cout<<"What product do you want to delete? Enter the name: ";
  string name;
  cin>>name;
  vector<Product*> updatedCart;
  for(int i=0; i<cart.size();i++)
    if(cart[i]->getName() != name)
      updatedCart.push_back(cart[i]);
    
  cart = updatedCart;
  cout<<"\nProduct deleted succesfully!\n\n";
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
  cout<<"\n\t Welcome to "<<this->name<<" supermarket! Shopping made easy since "<<this->foundationYear<<" :)\n\n";
}
void Supermarket::addProduct(){
  cout<<"What type of product do you want to add?\n";
  cout<<"\t 1. Food\n\t 2. Household item\n\t 3. Electrical device\n\t 4. Electrical appliance\n";
  int choice;
  cin>>choice;
  Product* p;
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
    default:
      cout<<"Invalid choice. Please enter a number in range 1-4.";
      break;
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
  cout<<"\nProduct deleted succesfully!\n\n";
}

////////////////////////////////////////

int main(){
  Product* p1 = new Food("chips", 200, 3.5, Food::category::snacks, "28.04.2024", 560, 130, 0);
  Product* p2 = new HouseholdProduct("bleach", 100, 10.5, "chlorine", true);
  Product* p3 = new ElectricalProduct("TV", 35, 400, 2, 500);
  Product* p4 = new ElectricalAppliance("Vaccum Cleaner", 50, 200.8, "plastic", false, 1, 300, 100, {"Bluetooth conectivity", "Automatic shut-off"} );
  vector<Product*> l = {p1,p2,p3,p4};
  
  Supermarket s("Clevr", 1987, l);
  s.welcome();
  
  int k;
  cout<<"1. Admin\n2. Client\n";
  cin>>k;
  cout<<endl;
  if(k == 1){
    while(k == 1){
        cout << "1. Add product\n";
        cout << "2. Show products\n";
        cout << "3. Edit product\n";
        cout << "4. Delete product\n";
        cout << "5. Stop\n\n";
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
                k = 0;
                break;
            }
        }
    }
  }
  else if (k == 2){
    while(k == 2){
        Cart c;
        s.showProducts();
        cout << "1. Add product to the cart\n";
        cout << "2. Delete product from the cart\n";
        cout << "3. Place order\n\n";
        int choice;
        cin >> choice;
        switch(choice){
            case 1:{
                c.add(s);
                break;
            }
            case 2:{
                c.del();
                break;
            }
            case 3:{
                c.buy();
                k = 0;
                break;
            }
        }
    }
  }
  else cout<<"\t Invalid user!";
  delete[] p1,p2,p3,p4;
  return 0;
}