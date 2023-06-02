#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_USERNAME_LENGTH 30
#define MAX_PASSWORD_LENGTH 30
#define DATABASE_FILE "users.txt"
typedef struct
{ // username and password structure
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
} User;

// Function to check if same username already exist in database or not for sign up
int is_user_exist(const char *username)
{
    FILE *file = fopen(DATABASE_FILE, "r");//opening database file in read mode
    if (file == NULL)
    {
        return 0;
    }

    char line[MAX_USERNAME_LENGTH + MAX_PASSWORD_LENGTH + 2];
    while (fgets(line, sizeof(line), file) != NULL)//taking username and password from database file 
    {
        char *token = strtok(line, " ");//tokenising username and password
        if (token != NULL && strcmp(token, username) == 0)//checking for username
        {
            fclose(file);
            return 1;//user exists cant sign in
        }
    }

    fclose(file);//closing the database file
    return 0;//user dont exists can sign in
}

//Function to add user in to the database file
int add_user(const User *user)
{
    if (is_user_exist(user->username))//checking if the user is already exist or not
    {
        return 0;//cant add user to database
    }

    FILE *file = fopen(DATABASE_FILE, "a");
    if (file == NULL)
    {
        return 0;
    }

    fprintf(file, "%s %s\n", user->username, user->password);//appending username and password into database file
    fclose(file);
    return 1;//user is added to database
}

//Function to check whether username and password are in database file for login to proceed
int authenticate(const User *user)
{
    FILE *file = fopen(DATABASE_FILE, "r");
    if (file == NULL)
    {
        return 0;
    }

    char line[MAX_USERNAME_LENGTH + MAX_PASSWORD_LENGTH + 2];
    while (fgets(line, sizeof(line), file) != NULL)//reading database file
    {
        char *saved_username = strtok(line, " ");//separating username and password as tokens
        char *saved_password = strtok(NULL, " \n");

        if (saved_username != NULL && saved_password != NULL &&
            strcmp(saved_username, user->username) == 0 &&
            strcmp(saved_password, user->password) == 0)//comparing username and pasword for login
        {
            fclose(file);
            return 1;//user has account in database file
        }
    }

    fclose(file);
    return 0;//user dont have an account
}
//function to reset password if forgot password
int reset_password(const char *username, const char *new_password)
{
    FILE *file = fopen(DATABASE_FILE, "r");
    if (file == NULL)
    {
        return 0;
    }

    FILE *temp_file = fopen("temp.txt", "w");//creating a temp file in write mode
    if (temp_file == NULL)
    {
        fclose(file);
        return 0;
    }

    char line[MAX_USERNAME_LENGTH + MAX_PASSWORD_LENGTH + 2];
    while (fgets(line, sizeof(line), file) != NULL)//reading old username and password
    {
        char *saved_username = strtok(line, " ");//tokenising and saving old username and old password 
        char *saved_password = strtok(NULL, " \n");

        if (saved_username != NULL && saved_password != NULL &&
            strcmp(saved_username, username) == 0)//searchinf for username and old pasword
        {
            fprintf(temp_file, "%s %s\n", saved_username, new_password);//assigning new password to username
        }
        else
        {
            fprintf(temp_file, "%s %s\n", saved_username, saved_password);//else dont change 
        }
    }

    fclose(file);
    fclose(temp_file);

    if (remove(DATABASE_FILE) != 0)
    {
        return 0;
    }

    if (rename("temp.txt", DATABASE_FILE) != 0)
    {
        return 0;
    }

    return 1;
}

//function to clear the terminal by pressing enter
void newpage()
{
    printf("Press Enter to proceed...\n");
    while (getchar() != '\n')
        continue;
    getchar();
    system("clear");
}

//Function to read from file and to print in on terminal. 
int printParagraphFromFile(char *filename, int paragraphToRead)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Error opening the file.\n");
        return 0;
    }
    int paragraphCount = 1;//intializing paragraph count
    char line[300];//paragraph size

    while (fgets(line, sizeof(line), file))//navigating the required para
    {
        if (line[0] == '\n')//if encounter new line move to next para
        {
            paragraphCount++;
        }

        if (paragraphCount == paragraphToRead)//if paragraph to be readed is same as paragraph count
        {
            printf("%s", line);//print para
        }
    }

    fclose(file);
}
//function to print reviews from file
int review(char *filename)
{
    int i = 6;// from para 6 reviews will start
    while (i < 30)//while not reaching at last in file
    {
        printParagraphFromFile(filename, i);//print paragraph
        i++;
    }
    printf("\n");
    return 1;
}
//function to add name, date and rating in review while writing the review
void appendNameDate(char *f)
{
    const char *filename = f;
    FILE *file = fopen(f, "a");
    if (file == NULL)
    {
        printf("Failed to open the file.\n");
    }
    char text[100];
    printf("Enter your name: ");
    while (getchar() != '\n')
    {
        continue;
    }
    fgets(text, sizeof(text), stdin);//to get name from user
    printf("Press Enter to proceed...");
    while (getchar() != '\n')
    {
        continue;
    }
    fprintf(file, "\n%s", text);//to print that name in file
    printf("Enter month and year: ");
    fgets(text, sizeof(text), stdin);//to get month and year from user
    printf("Press Enter to proceed...");
    while (getchar() != '\n')
    {
        continue;
    }
    fprintf(file, "%s", text);//to print that month and year in file
    printf("Rating(e.g-3/5): ");
    fgets(text, sizeof(text), stdin);//to get rating from user
    printf("Press Enter to proceed...");
    while (getchar() != '\n')
    {
        continue;
    }
    fprintf(file, "RATING - %s", text);//to print that rating in file
    fclose(file);
}
//funtion to append review taken from user in file
void appendreview(const char *file_path, const char *paragraph)
{
    FILE *file = fopen(file_path, "a");
    if (file == NULL)
    {
        printf("Unable to open file: %s\n", file_path);
        return;
    }
    fprintf(file, "%s\n", paragraph);//printing that review in file
    fclose(file);
}
//function to write a review
void writereview(char *filename)
{
    const char *file_path = filename;
    printf("Enter Review (enter '$' in new line to finish):\n");
    char paragraph[300] = {0};//string to orint para in the file
    char buffer[300];//string in which para will be written
    while (fgets(buffer, sizeof(buffer), stdin) != NULL)//read the review for $
    {
        if (strcmp(buffer, "$\n") == 0)//if $ found exit writing review
        {
            break;
        }
        strcat(paragraph, buffer);//concatenate the review in buffer to paragraph 
    }
    appendreview(file_path, paragraph);
}
void whatYouWant(char *filename)
{
    printf("\n\n");
    system("clear");// Clear the screen and display choices
    printf("What you Want To Know :\n");
    printf("1) Description\n");
    printf("2) Details\n");
    printf("3) Reviews\n");
    printf("4) See More Reviews\n");
    printf("5) Write A Review\n");
    printf("ENTER ANY OTHER NUMBER TO NAVIGATE TO PREVIOUS MENU\n");
    printf("Enter Your Choice : ");
    int choice1;
    scanf("%d", &choice1);//enter choice for description, details, reviews of places
    printf("\n");
    switch (choice1)
    {
    case 1:
    {
        // Display description of that place
        system("clear");
        printParagraphFromFile(filename, 1);
        printParagraphFromFile(filename, 2);
        printf("\nPRESS 0 TO NAVIGATE TO PREVIOUS MENU\n");
        printf("PRESS 1 TO EXIT\n");
        int choice3;
        printf("Enter choice:\t");
        scanf("%d", &choice3);//enter choice 0 to previous menu and 1 to exit
        switch (choice3)
        {
        case 0:
            break;
        case 1:
            exit(1);
        default:
            printf("Invalid Choice\n\n");
            newpage();
        }
        break;
    }
    case 2:
    {
        // Display details of that place
        system("clear");
        printParagraphFromFile(filename, 1);
        printParagraphFromFile(filename, 3);
        printf("\n\n\nPRESS 0 TO NAVIGATE TO PREVIOUS MENU\n");
        printf("PRESS 1 TO EXIT\n");
        int choice3;
        printf("Enter choice:\t");
        scanf("%d", &choice3);//enter choice 0 to previous menu and 1 to exit
        switch (choice3)
        {
        case 0:
            break;
        case 1:
            exit(2);
        default:
            printf("Invalid Choice\n\n");
            newpage();
        }
        break;
    }
    case 3:
    {
        // Display reviews of that place
        system("clear");
        printParagraphFromFile(filename, 1);
        printParagraphFromFile(filename, 4);
        printParagraphFromFile(filename, 5);
        printf("\n\n\nPRESS 0 TO NAVIGATE TO PREVIOUS MENU\n");
        printf("PRESS 1 TO EXIT\n");
        int choice3;
        printf("Enter choice:\t");
        scanf("%d", &choice3);//enter choice 0 to previous menu and 1 to exit
        switch (choice3)
        {
        case 0:
            break;
        case 1:
            exit(3);
        default:
            printf("Invalid Choice\n\n");
            newpage();
        }
        break;
    }
    case 4:
    {
        // Display more reviews
        system("clear");
        printParagraphFromFile(filename, 1);
        review(filename);
        printf("\n\n\nPRESS 0 TO NAVIGATE TO PREVIOUS MENU\n");
        printf("PRESS 1 TO EXIT\n");
        int choice3;
        printf("Enter choice:\t");
        scanf("%d", &choice3);//enter choice 0 to previous menu and 1 to exit
        switch (choice3)
        {
        case 0:
            break;
        case 1:
            exit(4);
        default:
            printf("Invalid Choice\n\n");
            newpage();
        }
        break;
    }
    case 5:
    {
        //allow user to Write a review for that place
        system("clear");
        appendNameDate(filename);
        writereview(filename);
        printf("\n\n\nPRESS 0 TO NAVIGATE TO PREVIOUS MENU\n");
        printf("PRESS 1 TO EXIT\n");
        int choice3;
        printf("Enter choice:\t");
        scanf("%d", &choice3);//enter choice 0 to previous menu and 1 to exit
        switch (choice3)
        {
        case 0:
            break;
        case 1:
            exit(3);
        default:
            printf("Invalid Choice\n\n");
            newpage();
        }
        break;
    }
    default:
        break;
    }
}
int main()
{
    int choice;
    User user;
    printf("\t\t\t_______   ________  __        __    __  ______    \n");
    printf("\t\t\t/       \ /        |/  |      /  |  /  |/      |    \n");
    printf("\t\t\t$$$$$$$  |$$$$$$$$/ $$ |      $$ |  $$ |$$$$$$/       \n");
    printf("\t\t\t$$ |  $$ |$$ |__    $$ |      $$ |__$$ |  $$ |           \n");
    printf("\t\t\t$$ |  $$ |$$    |   $$ |      $$    $$ |  $$ |           \n");
    printf("\t\t\t$$ |  $$ |$$$$$/    $$ |      $$$$$$$$ |  $$ |           \n");
    printf("\t\t\t$$ |__$$ |$$ |_____ $$ |_____ $$ |  $$ | _$$ |_           \n");
    printf("\t\t\t$$    $$/ $$       |$$       |$$ |  $$ |/ $$   |        \n");
    printf("\t\t\t$$$$$$$/  $$$$$$$$/ $$$$$$$$/ $$/   $$/ $$$$$$/           \n");

    printf("\t\t\t ________  _______    ______   __     __  ________  __     \n");
    printf("\t\t\t/        |/       \  /      \ /  |   /  |/        |/  |    \n");
    printf("\t\t\t$$$$$$$$/ $$$$$$$  |/$$$$$$  |$$ |   $$ |$$$$$$$$/ $$ |      \n");
    printf("\t\t\t   $$ |   $$ |__$$ |$$ |__$$ |$$ |   $$ |$$ |__    $$ |      \n");
    printf("\t\t\t   $$ |   $$    $$< $$    $$ |$$  \ /$$/ $$    |   $$ |      \n");
    printf("\t\t\t   $$ |   $$$$$$$  |$$$$$$$$ | $$  /$$/  $$$$$/    $$ |      \n");
    printf("\t\t\t   $$ |   $$ |  $$ |$$ |  $$ |  $$ $$/   $$ |_____ $$ |_____ \n");
    printf("\t\t\t   $$ |   $$ |  $$ |$$ |  $$ |   $$$/    $$       |$$       |\n");
    printf("\t\t\t   $$/    $$/   $$/ $$/   $$/     $/     $$$$$$$$/ $$$$$$$$/ \n");

    printf("\t\t\t  ______   __    __  ______  _______   ________              \n");
    printf("\t\t\t /      \ /  |  /  |/      |/       \ /        |         \n");
    printf("\t\t\t/$$$$$$  |$$ |  $$ |$$$$$$/ $$$$$$$  |$$$$$$$$/            \n");
    printf("\t\t\t$$ | _$$/ $$ |  $$ |  $$ |  $$ |  $$ |$$ |__               \n");
    printf("\t\t\t$$ |/    |$$ |  $$ |  $$ |  $$ |  $$ |$$    |               \n");
    printf("\t\t\t$$ \__$$ |$$ \__$$ | _$$ |_ $$ |__$$ |$$ |_____              \n");
    printf("\t\t\t$$ |$$$$ |$$ |  $$ |  $$ |  $$ |  $$ |$$$$$/                 \n");
    printf("\t\t\t$$ |__$$ |$$ |__$$ | _$$ |_ $$ |__$$ |$$ |_____              \n");
    printf("\t\t\t$$    $$/ $$    $$/ / $$   |$$    $$/ $$       |             \n");
    printf("\t\t\t $$$$$$/   $$$$$$/  $$$$$$/ $$$$$$$/  $$$$$$$$/              \n");

    newpage();
    // first while loop starts
    while (1)
    {
        printf("\n");
        printf("\t\t\t\t\t\tLogin Page\n");
        printf("1. Sign up\n");
        printf("2. Login\n");
        printf("3. Forgot Password\n");
        printf("Press any other number to exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice); // enter choice in login page
        switch (choice)
        {
        case 1:
            // sign up
            printf("Enter username: ");
            scanf("%s", user.username);
            printf("Enter password: ");
            scanf("%s", user.password);
            if (add_user(&user))
            {
                printf("User created successfully.\n\n");
                newpage();
            }
            else
            {
                printf("Failed to create user as user is already signed up.\n\n\n");
                newpage();
            }
            goto loginmenu;
        case 2:
            // login
            printf("Enter username: ");
            scanf("%s", user.username);
            printf("Enter password: ");
            scanf("%s", user.password);
            printf("\n");
            if (authenticate(&user))
            {
                printf("Login successful.\n\n\n");
                newpage();
            }
            else
            {
                printf("Invalid username or password.\n\n\n");
                newpage();
                goto loginmenu;
            }
            break;
        case 3:
            // forgot password
            printf("Enter username: ");
            scanf("%s", user.username);
            printf("Enter new password: ");
            scanf("%s", user.password);
            printf("\n");
            if (reset_password(&user, &user.password))
            {
                printf("Password changed successfully.\n\n\n");
            }
            else
            {
                printf("Failed to change password. Please try again later.\n\n\n");
            }
            newpage();
            goto loginmenu;
        default:
            // exit to terminal
            printf("Invalid choice.\n\n\n");
            return 0;
            break;
        }
        while (1)
        { // second while loop starts
            system("clear");
            printf("\nHere are some categories to help you explore different aspects of Delhi!!!\n");
            // category menu
            printf("1.HISTORICAL MONUMENT\n");
            printf("2.RELIGIOUS PLACES\n");
            printf("3.MUSEUMS AND ART GALLERIES\n");
            printf("4.MARKET AND SHOPPING\n");
            printf("5.GARDENS AND PARKS\n");
            printf("6.LOG OUT\n");
            int choice1;
            printf("Enter your choice: ");
            scanf("%d", &choice1); // enter choice for categories
            printf("\n");
            while (1)
            {                    // third while loop starts
                switch (choice1) // category switch starts
                {
                case 1:
                    system("clear");
                    printf("\n");
                    // historical monuments menu
                    printf("Here Is A List Of Delhi's Historical Monuments!!!!\n\n");
                    printf("1) Red Fort\n");
                    printf("2) India Gate\n");
                    printf("3) Qutub Minar\n");
                    printf("4) Purana Quila\n");
                    printf("5) Jantar Mantar\n");
                    printf("6) Humayun Tomb\n");
                    printf("7) Hauz Khas Fort\n");
                    printf("8) Agrasen Ki Baoli\n");
                    printf("9) Tomb of Safdarjang\n");
                    printf("10) Feroz Shah Kotla Fort\n");
                    printf("0) NAVIGATE TO PREVIOUS MENU\n");
                    printf("\tEnter your choice: ");
                    scanf("%d", &choice); // enter choice for historical menuments
                    printf("\n\n");
                    switch (choice + 10)
                    {
                    case 11:
                    {
                        char *filename = "Red_Fort.txt";
                        whatYouWant(filename); // function call for description, details etc
                        goto placesmenu1;
                        break;
                    }
                    case 12:
                    {
                        char *filename = "India_Gate.txt";
                        whatYouWant(filename);
                        goto placesmenu1;
                        break;
                    }
                    case 13:
                    {
                        char *filename = "Qutub_Minar.txt";
                        whatYouWant(filename);
                        goto placesmenu1;
                        break;
                    }
                    case 14:
                    {
                        char *filename = "Purana_Quila.txt";
                        whatYouWant(filename);
                        goto placesmenu1;
                        break;
                    }
                    case 15:
                    {
                        char *filename = "Jantar_Mantar.txt";
                        whatYouWant(filename);
                        goto placesmenu1;
                        break;
                    }
                    case 16:
                    {
                        char *filename = "Humayun_Tomb.txt";
                        whatYouWant(filename);
                        goto placesmenu1;
                        break;
                    }
                    case 17:
                    {
                        char *filename = "Hauz.txt";
                        whatYouWant(filename);
                        goto placesmenu1;
                        break;
                    }
                    case 18:
                    {
                        char *filename = "Agrasen.txt";
                        whatYouWant(filename);
                        goto placesmenu1;
                        break;
                    }
                    case 19:
                    {
                        char *filename = "Tomb_Of_Safdarjung.txt";
                        whatYouWant(filename);
                        goto placesmenu1;
                        break;
                    }
                    case 20:
                    {
                        char *filename = "Feroz_Shah_Kotla_Fort.txt";
                        whatYouWant(filename);
                        goto placesmenu1;
                        break;
                    }
                    case 10:
                        goto categorymenu;
                    default:
                        printf("Invalid choice.\n\n");
                        newpage();
                    }
                    break;
                case 2:
                    system("clear");
                    printf("\n");
                    // religious places menu
                    printf("Here Is A List Of Delhi's Religious Places!!!!\n\n");
                    printf("1) Jama Masjid\n");
                    printf("2) ISKCON Temple\n");
                    printf("3) Chattarpur Temple\n");
                    printf("4) Akshardham Temple\n");
                    printf("5) Bangla Sahib Gurudwara\n");
                    printf("6) Lotus Temple\n");
                    printf("7) NAVIGATE TO PREVIOUS MENU\n");
                    printf("\tEnter your choice: ");
                    scanf("%d", &choice); // enter choice for religious places
                    printf("\n");
                    switch (choice + 20)
                    {
                    case 21:
                    {
                        char *filename = "Jama_Masjid.txt";
                        whatYouWant(filename);
                        goto placesmenu2;
                        break;
                    }
                    case 22:
                    {
                        char *filename = "Iskcon.txt";
                        whatYouWant(filename);
                        goto placesmenu2;
                        break;
                    }
                    case 23:
                    {
                        char *filename = "Chattarpur_Temple.txt";
                        whatYouWant(filename);
                        goto placesmenu2;
                        break;
                    }
                    case 24:
                    {
                        char *filename = "Akshardham_Temple.txt";
                        whatYouWant(filename);
                        goto placesmenu2;
                        break;
                    }
                    case 25:
                    {
                        char *filename = "Bangla.txt";
                        whatYouWant(filename);
                        goto placesmenu2;
                        break;
                    }
                    case 26:
                    {
                        char *filename = "Lotus_Temple.txt";
                        whatYouWant(filename);
                        goto placesmenu2;
                        break;
                    }
                    case 27:
                        goto categorymenu;
                    default:
                        printf("Invalid choice.\n\n");
                        newpage();
                    }
                    break;
                case 3:
                    system("clear");
                    printf("\n");
                    // museums and art galleries menu
                    printf("Here Is A List Of Delhi's Museums And Art Galleries!!!!\n\n");
                    printf("1) National Rail Museum\n");
                    printf("2) Nehru Planeterium\n");
                    printf("3) NAVIGATE TO PREVIOUS MENU\n");
                    printf("Enter your choice: ");
                    scanf("%d", &choice); // enter choice for museums and art galleries
                    printf("\n");
                    switch (choice + 30)
                    {
                    case 31:
                    {
                        char *filename = "National_Rail_Museum.txt";
                        whatYouWant(filename);
                        goto placesmenu3;
                        break;
                    }
                    case 32:
                    {
                        char *filename = "Nehru_Planetarium.txt";
                        whatYouWant(filename);
                        goto placesmenu3;
                        break;
                    }
                    case 33:
                        goto categorymenu;
                    default:
                        printf("Invalid choice.\n\n");
                        newpage();
                    }
                    break;
                case 4:
                    system("clear");
                    printf("\n");
                    // popular markets menu
                    printf("Here Is A List Of Delhi's Popular Markets!!!!\n\n");
                    printf("1) Palika Bazaar\n");
                    printf("2) Chandni Chowk\n");
                    printf("3) Dilli Haat\n");
                    printf("4) Sarojni Nagar Market\n");
                    printf("5) Connaught Place\n");
                    printf("6) NAVIGATE TO PREVIOUS MENU\n");
                    printf("Enter your choice: ");
                    scanf("%d", &choice); // enter choice for popular markets
                    printf("\n");
                    switch (choice + 40)
                    {
                    case 41:
                    {
                        char *filename = "Palika_Bazaar.txt";
                        whatYouWant(filename);
                        goto placesmenu4;
                        break;
                    }
                    case 42:
                    {
                        char *filename = "Chandni_Chowk.txt";
                        whatYouWant(filename);
                        goto placesmenu4;
                        break;
                    }
                    case 43:
                    {
                        char *filename = "Dilli_Haat.txt";
                        whatYouWant(filename);
                        goto placesmenu4;
                        break;
                    }
                    case 44:
                    {
                        char *filename = "Sarojini_Nagar_Market.txt";
                        whatYouWant(filename);
                        goto placesmenu4;
                        break;
                    }
                    case 45:
                    {
                        char *filename = "Connaught_Place.txt";
                        whatYouWant(filename);
                        goto placesmenu4;
                        break;
                    }
                    case 46:
                        goto categorymenu;
                    default:
                        printf("Invalid choice.\n\n");
                        newpage();
                    }
                    break;
                case 5:
                    system("clear");
                    printf("\n");
                    // gardens and parks menu
                    printf("Here Is A List Of Delhi's Popular Garden And Parks!!!!\n\n");
                    printf("1) Lodhi Garden\n");
                    printf("2) National Zoological Park\n");
                    printf("3) Mughal Garden\n");
                    printf("4) NAVIGATE TO PREVIOUS MENU\n");
                    printf("Enter your choice: ");
                    scanf("%d", &choice); // enter choice for garden and parks
                    printf("\n");
                    switch (choice + 50)
                    {
                    case 51:
                    {
                        char *filename = "Lodhi.txt";
                        whatYouWant(filename);
                        goto placesmenu5;
                        break;
                    }
                    case 52:
                    {
                        char *filename = "National_Zoological_Park.txt";
                        whatYouWant(filename);
                        goto placesmenu5;
                        break;
                    }
                    case 53:
                    {
                        char *filename = "Mughal.txt";
                        whatYouWant(filename);
                        goto placesmenu5;
                        break;
                    }
                    case 54:
                        goto categorymenu;
                    default:
                        printf("Invalid choice.\n\n");
                        newpage();
                    }
                    break;
                case 6:
                    system("clear");
                    // to go to login menu
                    goto loginmenu;
                default:
                    // invalid choice for default
                    printf("Invalid choice.\n\n");
                    newpage();
                    goto categorymenu;

                } // category switch ends
                  //  placesmenu declarations to go to respective placesmenu
            placesmenu1:
                continue;
            placesmenu2:
                continue;
            placesmenu3:
                continue;
            placesmenu4:
                continue;
            placesmenu5:
                continue;
            } // third while loop ends
        // categorymenu declaration to go to category menu
        categorymenu:
            continue;
        } // second ehile loop ends
          //  loginmenu declaration to go to login menu
    loginmenu:
        continue;
    } // fiest while loop ends
    return 0;
}
