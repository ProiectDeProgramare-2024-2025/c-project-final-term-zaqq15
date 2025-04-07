# C Project / Final Term - 20 Points
## EN
For the final term of the C project, you must implement the whole application (mandatory to obtain the following points):

### (5p) Validations
> [!note]
> Validate all the inputs when requiring them.<br/>
> Be explicit, add the expected format.

#### Example

```cpp
'🚫 Please input date:'
// User doesn't know the expected format
// User doesn't know what the date is for

'✅ Date of aquisition (dd/mm/yyyy):'
// User knows the format
// User knows the date refers to the aquisition date
```


### (5p) Colors
> [!note]
> Add colors to the app.<br/>
> Colors must not take the whole line.<br/>
> Color only some elements, like the name of the product, parking lot etc. depending on the project.

#### Example

🚫 $${\color{red}4. \space Kiwi - 0.68eur \space \(7 pieces\)}$$<br/>
```cpp
// All the line is colored. This is the same as all being white, so it's not counted as you using colors
```

✅ $${\color{white}4. \space \color{lightblue}Kiwi \color{white}- \color{lightgreen}0.68\color{white}eur \space \(\color{yellow}7 \color{white}pieces\)}$$<br/>
```cpp
// Only the important elements are colored: product name, price and amount
```

### (5p) Structs
> [!note]
> Use C's `struct` to store in app data. For example:
```cpp
struct Product {
  char name[100];
  int amount;
  float price;
}
```

### (5p) Functions
> [!note]
> Extract core logic to separate functions.<br/>
> For example: `read_file()`, `write_file()`, `add_product(char* name, int amount, ...)` etc.

## RO
Trebuie implementată aplicația integral pentru termenul final al proiectului C (obligatoriu, pentru obținerea următoarelor puncte):

### (5p) Validări
> [!note]
> Validați datele când le cereți.<br/>
> Fiți expliciți în ceea ce cereți utilizatorului aplicației.

#### Exemplu

```cpp
'🚫 Introdu data:'
// Utilizatorul nu știe formatul în care se vrea introdusă data
// Utilizatorul nu știe pentru ce este introdusă data

'✅ Data achiziționării (zz/ll/aaaa):'
// Utilizatorul știe formatul
// Utilizatorul știe că data la care se face referire este data achiziționării
```


### (5p) Culori
> [!note]
> Adăugați culori aplicației.<br/>
> Culorile nu trebuie să fie pe întreaga linie.<br/>
> Colorați doar anumite elemente precum numele produsului, numele străzii locului de parcare etc. în funcție de ce se cere în proiect.

#### Exemplu

🚫 $${\color{red}4. \space Kiwi - 1.05RON \space \(7 bucăți\)}$$<br/>
```cpp
// Linia este colorată în întregime. E la fel ca în cazul în care linia era albă, acest mod nu se contorizează ca folosire de culori.
```

✅ $${\color{white}4. \space \color{lightblue}Kiwi \color{white}- \color{lightgreen}1.05\color{white}RON \space \(\color{yellow}7 \color{white}bucăți\)}$$<br/>
```cpp
// Doar elementele importante sunt colorate: numele, prețul și cantitatea produsului
```

### (5p) Structuri
> [!note]
> Folosiți `struct` din C pentru stocharea de date în aplicație. De examplu:
```cpp
struct Product {
  char name[100];
  int amount;
  float price;
}
```

### (5p) Funcții
> [!note]
> Extrageți funcționalitatea de bază a aplicației în funcții separate.<br/>
> De exemplu: `read_file()`, `write_file()`, `add_product(char* name, int amount, ...)` etc.
