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
  friend istream& operator >>(istream&, Product&);
  friend ostream& operator <<(ostream&, const Product&);
  istream& read(istream&);
  ostream& print(ostream&) const;
  virtual bool isOnSale() const = 0;
  virtual ~Product(){};
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
  friend istream& operator >>(istream&, Food&);
  friend ostream& operator <<(ostream&, const Food&);
  istream& read(istream&);
  ostream& print(ostream&) const;
  bool isOnSale() const;
  ~Food(){}
};

// class HouseholdProduct: virtual public Product{
//   string material;
//   bool hazardous;
// public:
//   HouseholdProduct();
//   HouseholdProduct(string, int, double, string, bool);
//   HouseholdProduct(const HouseholdProduct&);
//   HouseholdProduct& operator=(const HouseholdProduct&);
//   virtual ~HouseholdProduct(){}
// };

// class ElectricalProduct: virtual public Product{
//   int warranty;
//   int powerConsumption;
//   ElectricalProduct();
//   ElectricalProduct(string, int, double, int, int);
//   ElectricalProduct(const ElectricalProduct&);
//   ElectricalProduct& operator=(const ElectricalProduct&);
//   virtual ~ElectricalProduct(){}
// };

// class ElectricalAppliance: public HouseholdProduct, public ElectricalProduct{
//   int noiseLevel;
//   vector<string> features; 
//  //self-cleaning, automatic shut-off, water resistance, wi-fi, bluetooth
// };
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
  out<<"Price: "<<this->price<<"$"<<endl;
  return out;
}
istream& operator>>(istream& in, Product& p){
  return p.read(in);
}
ostream& operator<<(ostream& out, const Product& p) {
  return p.print(out);
}
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
  // Food f;
  // cin>>f;
  // cout<<f;
  return 0;
}