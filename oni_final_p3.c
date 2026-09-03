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



