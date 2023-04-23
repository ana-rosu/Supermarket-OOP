#include <iostream>
#include <vector>
using namespace std;

class IOInterface;
class Supermarket;
class Order;
class Product;
class Food;
class HouseholdProduct;
class ElectricalProduct;
class ElectricalAppliance;

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
class Order{
  // string date;
  // float time;
  vector<Product*> productList;
};
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
  out<<"Hazardous: [0/1]"<<(this->hazardous == true ? "Yes" : "No")<<endl;
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
  cout<<"Enter features: ";
  //enter features
  return in;
} 
ostream& ElectricalAppliance::print(ostream& out) const {
  HouseholdProduct::print(out);
  out<<"Warranty years: "<<this->warrantyYears<<" yrs"<<endl;
  out<<"Power consumption: "<<this->powerConsumption<<" watts"<<endl;
  out<<"Noise level: "<<this->noiseLevel<<endl;
  out<<"Features: ";
  //print features
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

int main(){
  // ElectricalProduct h;
  // cin>>h;
  // cout<<h;
  // Food f("tomato",20,1.5,1, "10.10.2020",2,15,1);
  // cout<<f;
  //enum
  //functionalities
  //menu
  return 0;
}