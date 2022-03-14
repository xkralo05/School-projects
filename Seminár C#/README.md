# ICS projekt

# Cíl
Cílem je vytvořit použitelnou a snadno rozšiřitelnou aplikaci, která splňuje požadavky zadání. Aplikace nemá padat nebo zamrzávat, pokud uživatel vyplní něco špatně, upozorní ho validační hláškou.

# Zadání - Aplikace pro správu festivalu
Výsledná aplikace má sloužit pro správu festivalu. Pro zjednodušení si můžete představit, že vytváříte jednodušší desktopovou verzi aplikace k Vašemu oblíbenému festivalu.

---
## Data
V rámci dat, se kterými se bude pracovat budeme požadovat minimálně následující data.

### Hudební skupina
- Originální název
- Žánr
- Fotografie
- Země původu
- Dlouhý textový popis skupiny
- Krátký popis do programu
- Seznam vystoupení na festivalu

### Stage
- Název
- Textový popis dle kterého jej návštěvníci najdou
- Fotografie
- Seznam skupin, které na něm hrají

### Program festivalu
- Časové sloty pro jednotlivá vystoupení na jednotlivých stagích

---
## Funkcionalita
Aplikace bude obsahovat několik pohledů pro zobrazování a zadávání dat. 

Je požadováno **perzistentní** uložení dat. To znamená, že když se aplikace restartuje, tak nesmí o data přijít. Je nutno data ukládat za běhu aplikace, aby bylo možno demonstrovat, že když se například pomocí aplikace přidá nová skupina, tak se zobrazí v seznamu skupin (a podobně pro ostatní data).

Pro uložení zvolte (MSSQL databázi), kterou zpřístupníte pomocí Entity Framework Core.

Minimální rozsah, který je požadován v rámci projektu je popsán v této kapitole.

### Seznam skupin
Seznam bude obsahovat všechny skupiny dostupné v aplikaci. Bude možno se z něj překliknout na detail skupiny a na pohled pro přidání nové skupiny.

### Detail skupiny
Pohled zobrazuje detail jednotlivé skupiny se všemi informacemi o skupině (viz kapitolu Data).

### Editace skupiny
Pohled, který slouží na editaci skupiny. Může se využít na vytvoření nové skupiny nebo na editaci existující. Bude obsahovat všechny informace o skupině (viz kapitola Data).

### Seznam stagí
Pohled obsahuje všechny stage. Bude možno se z něj překliknout na detail stage a na pohled pro přidání nové stage.

### Detail stage
Detail stage - stránka zobrazuje všechny informace o konkrétní stage včetně seznamu skupinu, které na ni hrají (viz kapitola Data).

### Editace stage
Pohled, který slouží na editaci stage. Může se využít na vytvoření nové stage nebo na editaci existující. Bude obsahovat všechny informace o stage (viz kapitola Data).

### Program festivalu
Pohled na program festivalu s jednotlivými časovými sloty pro jednotlivé stage. Bude na něm vidět všechny vystoupení všech skupin v průběhu festivalu. Jedna skupina může na festivalu vystupovat i vícekrát. Časové sloty můžou být různě dlouhé a můžou být mezi nimi přestávky.

### Editace programu
Bude možné editovat jednotlivé časové sloty pro jednotlivé stage a přiřazovat skupiny do časových slotů.

---
## Architektura projektu
Na cvičeních Vám ukazujeme nějakou strukturu organizace kódu do logických vrstev a projektů se zapojením návrhových vzorů. Pokoušíme se vysvětlit proč je vzorový projekt takhle organizovaný a proč jsou zvoleny jednotlivé rozhodnutí.

Budeme tedy i po Vás chtít logické rozvržení projektu. Můžete využít to, jak je organizovaný vzorový projekt probíraný na cvičeních a inspirovat se tímto uspořádáním (můžete ho mít stejné, za to Vám rozhodně body nestrhnem). Nebo můžete využít i vlastní uspořádání - v tom případě ale po Vás budeme chtít vysvěltit proč jste němu přistoupili a čím se jeho jednotlivé aspekty řídí.

V každém případě ale budeme chtít aby výsledné řešení obsahovalo víc projektů a vrstev. Snažíme se Vám na tomto projektu ukázat aj nějakou základní architekturu SW projektu, aby jste si z toho odnesli i něco víc než jen to, že budete znát syntax jazyka C#. Na tenhle aspket tedy rozhodně bude brán zřetel ve všech fázích hodnocení projektu.

---
## Správa projektu - Azure DevOps
Při řešení projektu týmy využívají Azure DevOps a využívají GIT na sdílení kódu.

Z GITu *musí být viditelná postupná práce na projektu a spolupráce týmu*. Pokud uvidíme, že existuje malé množství nelogických a nepřeložitelných commitů tak nás bude zajímat, jak jste spolupracovali a může to vést na snížení bodového hodnocení. Výrazně doporučujeme používat mechanismu **pull-request**ů a dělat si vzájemně review kódu, který následně mergnete do master nebo main větve. Projekt vypracováváte jako tým a nesete tak **kolektivní odpovědnost** za kvalitu kódu, proto je dobré, aby kód před začleněním vidělo více párů očí a vzájemně jste si jej připomínkovali. 

### Fáze 1 – objektový návrh 
V téhle fázi se zaměříme na *datový návrh*. Vyžaduje se po Vás, aby datový návrh splňoval zadání a nevynechal žádnou podstatnou část. Zamyslete se nad vazbami mezi jednotlivými entitami v datovém modelu. V následující fázi budete entity nahrávat do databáze, takže myslete na jejich propojení již nyní. V této fázi budeme chtít, abyste **odevzdali kód**, kde budete mít *entitní třídy*, které budou obsahovat všechny vlastnosti, které budete dále potřebovat a vazby mezi třídami.

Hodnotíme:
-   logický návrh tříd
-   využití abstrakce, zapouzdření, polymorfismu - kde to bude dávat smysl a eliminuje duplicity
-   verzování v GITu po logických částech

---
### Fáze 2 – databáze, repozitáře a mapování

Vytvořte napojení datových tříd pomocí Entity Frameworku na databázi. 

Vytvořte tedy repozitářovou (Repository) vrstvu, která zapouzdří databázové entity a zpřístupní pouze data přemapovaná do modelů/DTO.

Protože nemáte zatím UI, funkčnost aplikace ověřte automatizovanými testy! Kde to dává logický smysl tvořte **UnitTesty**, pro propojení s databází vytvářejte **Integrační testy**. Pro všechny typy testů využijte libovolný framework, doporučujeme **xUnit**.

Dbejte také kvality Vašeho kódu. Od této fáze se hodnotí i tenhle atribut. Opravte si tedy předchozí kód dle zásad Clean Code a SOLID probíraných na cvičení a důsledně je dodržujte. Můžete si dopomoct např. rozšířením **Code Metrices**.

Hodnotíme:
- opravení chyb a zapracování připomínek, které jsme vám dali v rámci hodnocení fáze 1
- využití **Entity Framework Core (EF) Code First** na vytvoření databáze z tříd navržených ve fázi 1
- existující databázové migrace (alespoň InitialMigration)
- návrh a funkčnost repozitářů
- čistotu kódu
- pokrytí aplikace testy - ukážete tím, že repozitáře opravdu fungují
- dejte pozor na zapouzdření databázových entit pod vrstvou repozitáře (resp. fasády), který je nepropaguje výše, ale přemapovává na modely/DTO
- :warning: není možné vytvořit kolidující **časové sloty**  na jednom stage v programu

---
### Fáze 3 – WPF frontend, data binding
V této fázi se od Vás již požaduje vytvoření WPF aplikace. Napište backend aplikace (vytvoření ViewModelů), která bude napojena na Vámi navržené datové modely z 2. fáze, které jsou zapouzdřeny v repozitáři (resp. fasádě). A dále frontend (View), která bude zobrazovat data předpřipravená ve ViewModelech. Zamyslete se nad tím, jakým způsobem je vhodné jednotlivá data zobrazovat.

Využijte *binding* v XAML kódu (vyvarujte se code-behind). Účelem není jenom udělat aplikaci, která funguje, ale také aplikaci, která je správně navržena a může být dále jednoduše upravitelná a rozšířitelná. Dbejte tedy zásad probíraných ve cvičeních.

Hodnotíme:
- opravení chyb a zapracování připomínek, které jsme vám dali v rámci hodnocení fází 1 a 2
- funkčnost celé výsledné aplikace
- vytvoření View, View-Modely
- zobrazení jednotlivých informací dle zadání – seznam, detail…
- správné využití data-bindingu v XAML
- čistotu kódu

Doporučujeme (bonusové body):
- pokrytí ViewModelů testy
- vytvoření dobře vypadající a plně funkční aplikace
- plánování projektu (logická struktura rozložení práce)
- nastavení automatizovaného buildu (kód je přeložitelný, spouští se automatizované testy, pipeline nekončí chybou)