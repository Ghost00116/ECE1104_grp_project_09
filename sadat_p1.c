#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define FILE_NAME "medicine.txt"

 

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

 


#include "oni_final_p3.c"
#include "tashfia_p2.c"

