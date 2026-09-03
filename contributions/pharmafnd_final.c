#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define FILE_NAME "medicine_final.txt"



typedef enum
{
    OTC = 1,
    PRESCRIPTION = 2
} MedicineType;

typedef enum
{
    REGULAR_USER = 1,
    ADMINISTRATOR = 2
} UserRole;

typedef union
{
    int shelfNumber;
    int prescriptionCode;
} ExtraInfo;

typedef struct
{
    int id;
    char name[40];
    char generic[40];
    char company[40];

    float strength;
    float price;

    int stock;

    MedicineType type;

    ExtraInfo info;

    char expiryDate[11];

} Medicine;


Medicine *loadFromFile(int *count);

int saveToFile(Medicine *medicines, int count);

void addMedicine(Medicine **medicines, int *count);

void showMedicines(Medicine *medicines, int count);

void searchMedicine(Medicine *medicines, int count);

int containsIgnoreCase(const char *text, const char *query);

int selectMedicine(Medicine *medicines, int count);

void findAlternative(Medicine *medicines, int count);

void updateStock(Medicine *medicines, int count);

void showLowStock(Medicine *medicines, int count);

void showTotalMedicines(int count);

UserRole login(void);

void deleteMedicine(Medicine **medicines, int *count);

void generateInvoice(Medicine *medicines, int count);

void showStatistics(Medicine *medicines, int count);

int findByNameRecursive(Medicine *medicines,
                        int count,
                        char name[],
                        int index);

void clearBuffer(void);

int main()
{
    Medicine *medicines = NULL;
    int count = 0;
    int choice;
    int exitProgram = 0;
    int loggedIn;
    UserRole role;

    medicines = loadFromFile(&count);


    if (medicines == NULL)
    {
        fprintf(stderr,
                "\nCould not load %s. "
                "Make sure it exists and uses the correct format.\n",
                FILE_NAME);
        return 1;
    }


    while (!exitProgram)
    {
        role = login();
        loggedIn = 1;

        while (loggedIn && !exitProgram)
        {
            printf("\n============================================\n");
            printf("     PHARMAFIND - PHARMACY SEARCH SYSTEM\n");
            printf("============================================\n");

            if (role == ADMINISTRATOR)
            {
                printf("1. Add Medicine\n2. Show All Medicines\n");
                printf("3. Search Medicine\n4. Find Alternative Medicine\n");
                printf("5. Update Stock\n6. Show Low Stock Medicines\n");
                printf("7. Show Total Number of Medicines\n");
                printf("8. Delete Medicine\n9. Generate Invoice\n");
                printf("10. Statistics Dashboard\n11. Logout\n");
                printf("12. Save and Exit\n");
            }
            else
            {
                printf("1. Search Medicine\n");
                printf("2. Generate Invoice\n");
                printf("3. Logout\n");
                printf("4. Save and Exit\n");
            }

            printf("--------------------------------------------\n");
            printf("Enter choice: ");

            if (scanf("%d", &choice) != 1)
            {
                printf("\nInvalid input!\n");
                clearBuffer();
                continue;
            }
            clearBuffer();

            if (role == REGULAR_USER)
            {
                if (choice == 1)
                    searchMedicine(medicines, count);
                else if (choice == 2)
                    generateInvoice(medicines, count);
                else if (choice == 3)
                {
                    if (saveToFile(medicines, count))
                        printf("\nData saved. Logged out successfully.\n");
                    else
                        printf("\nWarning: data could not be saved. Logging out.\n");
                    loggedIn = 0;
                }
                else if (choice == 4)
                {
                    if (saveToFile(medicines, count))
                        printf("\nData saved successfully.\n");
                    else
                        printf("\nData could not be saved.\n");
                    exitProgram = 1;
                }
                else
                    printf("\nInvalid menu choice!\n");
                continue;
            }

            switch (choice)
            {
                case 1: addMedicine(&medicines, &count); break;
                case 2: showMedicines(medicines, count); break;
                case 3: searchMedicine(medicines, count); break;
                case 4: findAlternative(medicines, count); break;
                case 5: updateStock(medicines, count); break;
                case 6: showLowStock(medicines, count); break;
                case 7: showTotalMedicines(count); break;
                case 8: deleteMedicine(&medicines, &count); break;
                case 9: generateInvoice(medicines, count); break;
                case 10: showStatistics(medicines, count); break;
                case 11:
                    if (saveToFile(medicines, count))
                        printf("\nData saved. Logged out successfully.\n");
                    else
                        printf("\nWarning: data could not be saved. Logging out.\n");
                    loggedIn = 0;
                    break;
                case 12:
                    if (saveToFile(medicines, count))
                        printf("\nData saved successfully.\n");
                    else
                        printf("\nData could not be saved.\n");
                    exitProgram = 1;
                    break;
                default: printf("\nInvalid menu choice!\n");
            }
        }
    }


    free(medicines);
    printf("Program closed.\n");
    return 0;
}


void showTotalMedicines(int count)
{
    printf("\nTotal number of medicines: %d\n", count);
}

UserRole login(void)
{
    char username[30];
    char password[30];
    int attempts;

    printf("\nDemo accounts: admin/admin123 or user/user123\n");

    for (attempts = 0; attempts < 3; attempts++)
    {
        printf("Username: ");
        fgets(username, sizeof(username), stdin);
        username[strcspn(username, "\n")] = '\0';

        printf("Password: ");
        fgets(password, sizeof(password), stdin);
        password[strcspn(password, "\n")] = '\0';

        if (strcmp(username, "admin") == 0 &&
            strcmp(password, "admin123") == 0)
            return ADMINISTRATOR;

        if (strcmp(username, "user") == 0 &&
            strcmp(password, "user123") == 0)
            return REGULAR_USER;

        printf("Invalid login. %d attempt(s) remaining.\n", 2 - attempts);
    }

    printf("Too many failed login attempts.\n");
    exit(1);
}

void addMedicine(Medicine **medicines, int *count)
{
    Medicine *temp;


    temp = realloc(*medicines,
                   (*count + 1) * sizeof(Medicine));


    if (temp == NULL)
    {
        printf("\nMemory allocation failed!\n");

        return;
    }


    *medicines = temp;


    Medicine *m = &(*medicines)[*count];


    printf("\n========== ADD MEDICINE ==========\n");


    printf("Medicine ID: ");

    scanf("%d", &m->id);

    clearBuffer();

    for (int i = 0; i < *count; i++)
    {
        if ((*medicines)[i].id == m->id)
        {
            printf("\nA medicine with ID %d already exists.\n", m->id);
            return;
        }
    }


    printf("Medicine name: ");

    fgets(m->name, 40, stdin);

    m->name[strcspn(m->name, "\n")] = '\0';


    printf("Generic name: ");

    fgets(m->generic, 40, stdin);

    m->generic[strcspn(m->generic, "\n")] = '\0';


    printf("Company name: ");

    fgets(m->company, 40, stdin);

    m->company[strcspn(m->company, "\n")] = '\0';

    printf("Expiry date (YYYY-MM-DD): ");
    fgets(m->expiryDate, sizeof(m->expiryDate), stdin);
    m->expiryDate[strcspn(m->expiryDate, "\n")] = '\0';


    printf("Strength (mg): ");

    scanf("%f", &m->strength);


    printf("Price: ");

    scanf("%f", &m->price);


    printf("Stock: ");

    scanf("%d", &m->stock);


    printf("\n1. OTC\n");
    printf("2. Prescription\n");

    printf("Medicine type: ");

    scanf("%d", (int *)&m->type);


    if (m->type == OTC)
    {
        printf("Shelf number: ");

        scanf("%d",
              &m->info.shelfNumber);
    }

    else
    {
        printf("Prescription code: ");

        scanf("%d",
              &m->info.prescriptionCode);
    }


    clearBuffer();


    (*count)++;


    printf("\nMedicine added successfully!\n");
}

void showMedicines(Medicine *medicines, int count)
{
    int i;


    if (count == 0)
    {
        printf("\nNo medicines available.\n");

        return;
    }


    printf("\n====================================================================\n");
    printf("                         MEDICINE DATABASE\n");
    printf("====================================================================\n");


    for (i = 0; i < count; i++)
    {
        printf("\nID       : %d\n",
               medicines[i].id);

        printf("Name     : %s\n",
               medicines[i].name);

        printf("Generic  : %s\n",
               medicines[i].generic);

        printf("Company  : %s\n",
               medicines[i].company);

        printf("Strength : %.0f mg\n",
               medicines[i].strength);

        printf("Price    : %.2f\n",
               medicines[i].price);

        printf("Stock    : %d\n",
               medicines[i].stock);

        printf("Expiry   : %s\n",
               medicines[i].expiryDate);


        if (medicines[i].type == OTC)
        {
            printf("Type     : OTC\n");

            printf("Shelf No : %d\n",
                   medicines[i].info.shelfNumber);
        }

        else
        {
            printf("Type     : Prescription\n");

            printf("Code     : %d\n",
                   medicines[i].info.prescriptionCode);
        }


        printf("----------------------------------------\n");
    }
}

int findByNameRecursive(Medicine *medicines,
                        int count,
                        char name[],
                        int index)
{


    if (index >= count)
    {
        return -1;
    }

    if (strcmp(medicines[index].name,
               name) == 0)
    {
        return index;
    }

    return findByNameRecursive(medicines,
                               count,
                               name,
                               index + 1);
}


int containsIgnoreCase(const char *text, const char *query)
{
    int i;
    int j;

    if (query[0] == '\0')
        return 0;

    for (i = 0; text[i] != '\0'; i++)
    {
        for (j = 0;
             query[j] != '\0' && text[i + j] != '\0' &&
             tolower((unsigned char)text[i + j]) ==
             tolower((unsigned char)query[j]);
             j++)
        {

        }

        if (query[j] == '\0')
            return 1;
    }

    return 0;
}

int selectMedicine(Medicine *medicines, int count)
{
    char query[100];
    int matches = 0;
    int selected = -1;
    int requestedId;
    int isNumber = 1;
    int i;
    int j;

    printf("\nEnter an ID, name, generic name, or manufacturer: ");
    fgets(query, sizeof(query), stdin);
    query[strcspn(query, "\n")] = '\0';

    if (query[0] == '\0')
        return -1;

    for (j = 0; query[j] != '\0'; j++)
    {
        if (!isdigit((unsigned char)query[j]))
        {
            isNumber = 0;
            break;
        }
    }

    requestedId = isNumber ? atoi(query) : -1;

    for (i = 0; i < count; i++)
    {
        if ((isNumber && medicines[i].id == requestedId) ||
            containsIgnoreCase(medicines[i].name, query) ||
            containsIgnoreCase(medicines[i].generic, query) ||
            containsIgnoreCase(medicines[i].company, query))
        {
            printf("ID: %d | %s | %s | %s\n",
                   medicines[i].id,
                   medicines[i].name,
                   medicines[i].generic,
                   medicines[i].company);
            selected = i;
            matches++;
        }
    }

    if (matches == 0)
        return -1;

    if (matches == 1)
        return selected;

    printf("Multiple medicines matched. Enter the exact medicine ID: ");
    if (scanf("%d", &requestedId) != 1)
    {
        clearBuffer();
        return -1;
    }
    clearBuffer();

    for (i = 0; i < count; i++)
    {
        if (medicines[i].id == requestedId)
            return i;
    }

    return -1;
}

void searchMedicine(Medicine *medicines, int count)
{
    char query[100];
    int searchId = -1;
    int isNumber = 1;
    int found = 0;
    int i;
    int j;

    printf("\nSearch by name, ID, generic name, or manufacturer: ");
    fgets(query, sizeof(query), stdin);
    query[strcspn(query, "\n")] = '\0';

    if (query[0] == '\0')
    {
        printf("\nSearch cannot be empty.\n");
        return;
    }

    for (j = 0; query[j] != '\0'; j++)
    {
        if (!isdigit((unsigned char)query[j]))
        {
            isNumber = 0;
            break;
        }
    }

    if (isNumber)
        searchId = atoi(query);

    for (i = 0; i < count; i++)
    {
        if ((isNumber && medicines[i].id == searchId) ||
            containsIgnoreCase(medicines[i].name, query) ||
            containsIgnoreCase(medicines[i].generic, query) ||
            containsIgnoreCase(medicines[i].company, query))
        {
            printf("\n========== MEDICINE FOUND ==========\n");
            printf("ID           : %d\n", medicines[i].id);
            printf("Name         : %s\n", medicines[i].name);
            printf("Generic      : %s\n", medicines[i].generic);
            printf("Manufacturer : %s\n", medicines[i].company);
            printf("Strength     : %.0f mg\n", medicines[i].strength);
            printf("Price        : %.2f\n", medicines[i].price);
            printf("Stock        : %d\n", medicines[i].stock);
            printf("Expiry       : %s\n", medicines[i].expiryDate);
            printf("Status       : %s\n",
                   medicines[i].stock == 0 ? "OUT OF STOCK" : "AVAILABLE");
            printf("------------------------------------\n");
            found = 1;
        }
    }

    if (!found)
        printf("\nNo matching medicine found.\n");
}


void findAlternative(Medicine *medicines, int count)
{
    int position;
    int found = 0;
    int i;


    position = selectMedicine(medicines, count);


    if (position == -1)
    {
        printf("\nMedicine not found!\n");

        return;
    }


    printf("\nOriginal Medicine\n");

    printf("-----------------------------------\n");

    printf("Name     : %s\n",
           medicines[position].name);

    printf("Generic  : %s\n",
           medicines[position].generic);

    printf("Strength : %.0f mg\n",
           medicines[position].strength);

    printf("Stock    : %d\n",
           medicines[position].stock);


    printf("\nPossible Alternatives\n");

    printf("-----------------------------------\n");


    for (i = 0; i < count; i++)
    {
        /*
           Alternative condition:

           Same generic
           Same strength
           Different brand
           Stock available
        */

        if (i != position &&
            strcmp(medicines[i].generic,
                   medicines[position].generic) == 0 &&
            medicines[i].strength ==
                   medicines[position].strength &&
            medicines[i].stock > 0)
        {

            printf("\nName     : %s\n",
                   medicines[i].name);

            printf("Company  : %s\n",
                   medicines[i].company);

            printf("Price    : %.2f\n",
                   medicines[i].price);

            printf("Stock    : %d\n",
                   medicines[i].stock);

            printf("-----------------------------------\n");


            found = 1;
        }
    }


    if (found == 0)
    {
        printf("\nNo available alternative found.\n");
    }
}


void updateStock(Medicine *medicines, int count)
{
    int position;
    int amount;


    position = selectMedicine(medicines, count);


    if (position == -1)
    {
        printf("\nMedicine not found!\n");

        return;
    }


    printf("Current stock: %d\n",
           medicines[position].stock);


    printf("Enter new stock: ");

    if (scanf("%d", &amount) != 1)
    {
        printf("\nInvalid stock!\n");

        clearBuffer();

        return;
    }


    clearBuffer();


    if (amount < 0)
    {
        printf("\nStock cannot be negative!\n");

        return;
    }


    medicines[position].stock = amount;


    printf("\nStock updated successfully.\n");
}


void showLowStock(Medicine *medicines, int count)
{
    int i;

    int found = 0;


    printf("\n========== LOW STOCK MEDICINES ==========\n");


    for (i = 0; i < count; i++)
    {
        if (medicines[i].stock < 10)
        {
            printf("\nName    : %s\n",
                   medicines[i].name);

            printf("Generic : %s\n",
                   medicines[i].generic);

            printf("Stock   : %d\n",
                   medicines[i].stock);

            printf("-----------------------------------\n");


            found = 1;
        }
    }


    if (found == 0)
    {
        printf("\nNo low-stock medicine found.\n");
    }
}

void deleteMedicine(Medicine **medicines, int *count)
{
    int position = selectMedicine(*medicines, *count);
    int i;
    char confirm;

    if (position == -1)
    {
        printf("\nMedicine not found.\n");
        return;
    }

    printf("Delete %s (ID %d)? (y/n): ",
           (*medicines)[position].name,
           (*medicines)[position].id);
    scanf(" %c", &confirm);
    clearBuffer();

    if (tolower((unsigned char)confirm) != 'y')
    {
        printf("Deletion cancelled.\n");
        return;
    }

    for (i = position; i < *count - 1; i++)
        (*medicines)[i] = (*medicines)[i + 1];

    (*count)--;
    printf("Medicine deleted successfully.\n");
}

void generateInvoice(Medicine *medicines, int count)
{
    typedef struct
    {
        char name[40];
        int quantity;
        float unitPrice;
    } InvoiceItem;

    InvoiceItem *items = NULL;
    int itemCount = 0;
    int position;
    int quantity;
    char more = 'y';
    float total = 0.0f;
    int i;

    while (tolower((unsigned char)more) == 'y')
    {
        position = selectMedicine(medicines, count);
        if (position == -1)
        {
            printf("Medicine not found.\n");
        }
        else
        {
            printf("Quantity: ");
            if (scanf("%d", &quantity) != 1 || quantity <= 0)
            {
                printf("Invalid quantity.\n");
                clearBuffer();
            }
            else
            {
                clearBuffer();
                if (quantity > medicines[position].stock)
                {
                    printf("Only %d unit(s) are available.\n",
                           medicines[position].stock);
                }
                else
                {
                    InvoiceItem *temp = realloc(
                        items, (itemCount + 1) * sizeof(InvoiceItem));
                    if (temp == NULL)
                    {
                        printf("Could not allocate invoice memory.\n");
                        free(items);
                        return;
                    }

                    items = temp;
                    strcpy(items[itemCount].name, medicines[position].name);
                    items[itemCount].quantity = quantity;
                    items[itemCount].unitPrice = medicines[position].price;
                    medicines[position].stock -= quantity;
                    total += quantity * medicines[position].price;
                    itemCount++;
                }
            }
        }

        printf("Add another medicine? (y/n): ");
        scanf(" %c", &more);
        clearBuffer();
    }

    if (itemCount == 0)
    {
        printf("\nInvoice contains no items.\n");
        free(items);
        return;
    }

    printf("\n================================================\n");
    printf("                 PHARMAFIND RECEIPT\n");
    printf("================================================\n");
    printf("%-20s %5s %12s %13s\n",
           "Medicine", "Qty", "Price (Tk)", "Subtotal (Tk)");

    for (i = 0; i < itemCount; i++)
    {
        printf("%-20s %5d %12.2f %13.2f\n",
               items[i].name,
               items[i].quantity,
               items[i].unitPrice,
               items[i].quantity * items[i].unitPrice);
    }

    printf("------------------------------------------------\n");
    printf("TOTAL: Tk %.2f\n", total);
    printf("================================================\n");
    free(items);
}

void showStatistics(Medicine *medicines, int count)
{
    int totalUnits = 0;
    int lowStock = 0;
    int outOfStock = 0;
    int otcCount = 0;
    int prescriptionCount = 0;
    float inventoryValue = 0.0f;
    int i;

    for (i = 0; i < count; i++)
    {
        totalUnits += medicines[i].stock;
        inventoryValue += medicines[i].stock * medicines[i].price;

        if (medicines[i].stock == 0)
            outOfStock++;
        else if (medicines[i].stock < 10)
            lowStock++;

        if (medicines[i].type == OTC)
            otcCount++;
        else
            prescriptionCount++;
    }

    printf("\n========== STATISTICS DASHBOARD ==========\n");
    printf("Total medicines              : %d\n", count);
    printf("Total stock units            : %d\n", totalUnits);
    printf("Total inventory value        : %.2f\n", inventoryValue);
    printf("Low-stock products (1-9)     : %d\n", lowStock);
    printf("Out-of-stock products        : %d\n", outOfStock);
    printf("OTC products                 : %d\n", otcCount);
    printf("Prescription products        : %d\n", prescriptionCount);
}

int saveToFile(Medicine *medicines, int count)
{
    FILE *file;

    int i;


    file = fopen(FILE_NAME, "w");


    if (file == NULL)
    {
        printf("\nFile opening error!\n");

        return 0;
    }


    fprintf(file, "%d\n", count);


    for (i = 0; i < count; i++)
    {
        fprintf(file,
                "%d|%s|%s|%s|%.2f|%.2f|%d|%d|%d|%s\n",

                medicines[i].id,
                medicines[i].name,
                medicines[i].generic,
                medicines[i].company,
                medicines[i].strength,
                medicines[i].price,
                medicines[i].stock,
                medicines[i].type,

                medicines[i].type == OTC
                ? medicines[i].info.shelfNumber
                : medicines[i].info.prescriptionCode,
                medicines[i].expiryDate);
    }


    fclose(file);
    return 1;
}


Medicine *loadFromFile(int *count)
{
    FILE *file;

    Medicine *medicines;

    int i;
    int type;
    int extra;
    int fieldsRead;
    char line[300];


    file = fopen(FILE_NAME, "r");

    if (file == NULL)
    {
        return NULL;
    }


    if (fscanf(file, "%d\n", count) != 1)
    {
        fclose(file);

        return NULL;
    }


    medicines =
        malloc((*count) * sizeof(Medicine));


    if (medicines == NULL)
    {
        fclose(file);

        return NULL;
    }


    for (i = 0; i < *count; i++)
    {
        if (fgets(line, sizeof(line), file) == NULL)
        {
            free(medicines);
            fclose(file);
            return NULL;
        }

        fieldsRead = sscanf(line,
                            "%d|%39[^|]|%39[^|]|%39[^|]|%f|%f|%d|%d|%d|%10[^\r\n]",
                            &medicines[i].id,
                            medicines[i].name,
                            medicines[i].generic,
                            medicines[i].company,
                            &medicines[i].strength,
                            &medicines[i].price,
                            &medicines[i].stock,
                            &type,
                            &extra,
                            medicines[i].expiryDate);

        if (fieldsRead != 9 && fieldsRead != 10)
        {
            free(medicines);
            fclose(file);
            return NULL;
        }

        if (fieldsRead == 9)
            strcpy(medicines[i].expiryDate, "N/A");


        medicines[i].type =
            (MedicineType)type;


        if (medicines[i].type == OTC)
        {
            medicines[i].info.shelfNumber =
                extra;
        }

        else
        {
            medicines[i].info.prescriptionCode =
                extra;
        }
    }


    fclose(file);


    return medicines;
}

void clearBuffer(void)
{
    int c;


    while ((c = getchar()) != '\n' &&
           c != EOF)
    {

    }
}


