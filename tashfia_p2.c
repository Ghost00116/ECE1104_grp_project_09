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
