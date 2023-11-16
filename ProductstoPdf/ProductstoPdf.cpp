#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <hpdf.h>
#include <setjmp.h>
#include <stdio.h>

using namespace std;

struct Product {
    int id;
    string name;
    double price;
    int quantity;

    Product(int id, string name, double price, int quantity)
        : id(id), name(name), price(price), quantity(quantity) {}

};
int cont = 0;

void addProduct(vector<Product>& products) {
    int id;
    string name;
    double price;
    int quantity;
    system("cls||clear");
    id = ++cont;
    cout << "Id: " << id << endl;
    cout << "Enter product name: ";
    cin >> name;
    cout << "Enter product price: ";
    cin >> price;
    cout << "Enter product quantity: ";
    cin >> quantity;

    products.push_back(Product(id, name, price, quantity));
    system("pause");
}

void showProducts(vector<Product>& products) {
    system("cls||clear");
    for (auto& product : products) {
        cout << "Id: " << product.id << endl;
        cout << "Name: " << product.name << endl;
        cout << "Price: " << product.price << endl;
        cout << "Quantity: " << product.quantity << endl;
        cout << endl;
    }
    system("pause");
}

void error_handler(HPDF_STATUS error_no, HPDF_STATUS detail_no, void* user_data)
{
    cerr << "ERROR: " << error_no << ", detail: " << detail_no << std::endl;
    longjmp(*(jmp_buf*)user_data, 1);
}


void exportListProductsPDF(const std::vector<Product>& products) {
    HPDF_Doc pdf = HPDF_New(error_handler, NULL);
    if (!pdf) {
        cerr << "ERROR: Can't create PdfDoc object\n";
        return;
    }

    HPDF_Page page = HPDF_AddPage(pdf);
    HPDF_Page_SetSize(page, HPDF_PAGE_SIZE_A4, HPDF_PAGE_PORTRAIT);

    HPDF_Font font = HPDF_GetFont(pdf, "Helvetica", NULL);
    HPDF_Page_SetFontAndSize(page, font, 20);

    HPDF_Page_BeginText(page);
    HPDF_Page_TextOut(page, 50, HPDF_Page_GetHeight(page) - 50, "Products");
    HPDF_Page_EndText(page);

    HPDF_Page_SetFontAndSize(page, font, 13);
    float y = HPDF_Page_GetHeight(page) - 80; // Posición inicial en Y

    string header = "ID  Nombre  Precio  Stock";
    HPDF_Page_BeginText(page);
    HPDF_Page_TextOut(page, 50, y, header.c_str());
    HPDF_Page_EndText(page);

    y = HPDF_Page_GetHeight(page) - 100;
    for (const auto& product : products) {
        string line = to_string(product.id) + " " + product.name +
            " " + to_string(product.price) + " " + to_string(product.quantity);

        HPDF_Page_BeginText(page);
        HPDF_Page_TextOut(page, 50, y, line.c_str());
        HPDF_Page_EndText(page);

        y -= 20; // Espacio entre líneas
        if (y < 50) { // Crear nueva página si no hay espacio
            page = HPDF_AddPage(pdf);
            HPDF_Page_SetSize(page, HPDF_PAGE_SIZE_A4, HPDF_PAGE_PORTRAIT);
            HPDF_Page_SetFontAndSize(page, font, 12);
            y = HPDF_Page_GetHeight(page) - 50;
        }
    }

    std::string footer = "Register: " + to_string(products.size());
    HPDF_Page_BeginText(page);
    HPDF_Page_TextOut(page, 50, 50, footer.c_str());
    HPDF_Page_EndText(page);



    HPDF_SaveToFile(pdf, "Products.pdf");
    HPDF_Free(pdf);

    std::cout << "Products exported to 'Products.pdf'\n";
    system("pause");
}

int showMenu() {
    system("cls||clear");
    int option;
    cout << "1. Add product" << endl;
    cout << "2. Show products" << endl;
    cout << "3. Export products to PDF" << endl;
    cout << "4. Exit" << endl;
    cout << "Enter option: ";
    cin >> option;
    return option;
}

int main()
{
    vector<Product> products;
    int option;
    do {
        option = showMenu();
        switch (option) {
        case 1:
            addProduct(products);
            break;
        case 2:
            showProducts(products);
            break;
        case 3:
            exportListProductsPDF(products);
            break;
        case 4:
            cout << "Bye!" << endl;
            break;
        default:
            cout << "Invalid option" << endl;
            break;
        }
    } while (option != 4);
}

