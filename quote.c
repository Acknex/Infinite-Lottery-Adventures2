#define NUMQUOTES 22
#define NUMSOUNDS 4

STRING* strQuote[NUMQUOTES];
SOUND* sndQuote[NUMSOUNDS];
TEXT* txtQuote;
var vTextOwner;
FONT* fntQuote = "Times New Roman#24b"; 
VECTOR vecTmp;




action quote()
{
	vec_scale(my->scale_x, 0.5);

	vec_set (vecTmp, my->x);
	c_setminmax(my);
	wait(1);

	vecTmp.z -= 3000;
	c_trace (my->x, vecTmp, IGNORE_ME|IGNORE_PASSABLE|IGNORE_SPRITES); 
	my->z = target.z - my->min_z; 
	my->trigger_range = 50;

	my->emask |= ENABLE_TRIGGER;
	my->flags |= LIGHT | PASSABLE;
	my->event = quote_event;

	wait(1);
	my->skill97 = handle(ent_create("quotelight.mdl", my->x, quotelight));	
	my->skill98 = handle(ent_create("quotelight.mdl", my->x, quotelight2));	

	while (me != NULL)
	{
		my->red = 255;
		wait (1);
	}
}


action quotelight()
{
	vec_set (vecTmp, my->x);
	c_setminmax(my);
	wait(1);

	my->z += 20;
	vecTmp.z -= 3000;
	c_trace (my->x, vecTmp, IGNORE_ME|IGNORE_PASSABLE|IGNORE_SPRITES); 
	my->z = target.z - my->min_z; 
	my->flags |= TRANSLUCENT | BRIGHT | PASSABLE;

	while (me != NULL)
	{
		my->skill99 += 7* time_step;
		my->pan += 3 * time_step;
		my->alpha = 50 + 25 * sinv(my->skill99);
		wait (1);
	}
}

action quotelight2()
{
	vec_set (vecTmp, my->x);
	my->scale_x = 0.3;
	my->scale_y = 0.3;
	my->scale_z = 1.2;
	c_setminmax(my);
	wait(1);

	my->z += 20;
	vecTmp.z -= 3000;
	c_trace (my->x, vecTmp, IGNORE_ME|IGNORE_PASSABLE|IGNORE_SPRITES); 
	my->z = target.z - my->min_z; 
	my->flags |= TRANSLUCENT | BRIGHT | PASSABLE;

	while (me != NULL)
	{
		my->skill99 += 5* time_step;
		my->pan -= 3 * time_step;
		my->alpha = 90;
		my->scale_x = 0.5 + 0.15 * cosv(my->skill99);
		my->scale_y = my->scale_x;
		wait (1);
	}
}


void quote_event()
{
	ENTITY* ent;
	var i;
	
	if ((event_type == EVENT_TRIGGER) && (my->skill1 < NUMQUOTES))
	{
		vTextOwner = my->skill1;
		ent = ptr_for_handle(my->skill97);
		ent->flags |= INVISIBLE;
		ent = ptr_for_handle(my->skill98);
		ent->flags |= INVISIBLE;
		my->event = NULL;
		my->emask &= ~ENABLE_TRIGGER;
		my->trigger_range = 0;

		(txtQuote->pstring)[0] = strQuote[my->skill1];
		txtQuote->pos_x = screen_size.x / 2;
		txtQuote->flags |= SHOW;
		i = absv(integer(random(NUMSOUNDS)));	
		snd_play(sndQuote[i], 100, 0);
		ent_create("quotescore.tga", ptrActor->x, quote_score);
		wait (-10);
		if (my->skill1 == vTextOwner)
			txtQuote->flags &= ~SHOW;
	}
}

void quote_score()
{
	var vCount = 0;
	
	my->flags |= BRIGHT | PASSABLE;
	my->ambient = 100;
	my->scale_x = 0.5;
	my->skill90 = my->x;
	my->skill91 = my->y;
	
	
	points += 3;
	
	while (my->alpha > 0)
	{
		wait (1);
		if (my->skill1 == 0)
		{
			my->scale_x += 0.1 * time_step;
			if (my->scale_x >= 0.8)
			{
				my->scale_x = 0.8;
				my->skill1 = 1;
			}
		}
		else if (my->scale_x > 0.1)
		{
			my->scale_x -= 0.05 * time_step;
			my->scale_y -= 0.05 * time_step;
		}
		else
		{
			my->scale_x = 0.1;
			my->scale_y = 0.1;
		}
		
		my->alpha -= 3 * time_step;
		my->z += 4* time_step;
		my->x = my->skill90 + 0.4*vCount * sinv(vCount*5);
		my->y = my->skill91 + 0.4*vCount * sinv(vCount*5);
		vCount += 4* time_step;	
	}
	my->alpha = 0;
	ptr_remove(me);
}

void create_quotes()
{	
	var i;
	STRING* strFile = str_create("");
	STRING* strNum = str_create("");
	
	for (i = 0; i < NUMSOUNDS; i++)
	{
		str_cpy(strFile, "sndquote");
		str_for_num(strNum, i+1);
		str_cat(strFile, strNum);
		str_cat(strFile, ".ogg");
		sndQuote[i] = snd_create(strFile);
	}
	
	strQuote[0] = str_create("USER: Will this be changed in the future? 
JCL: Yes, certainly, but not very soon. This is in a part of the compiler that I haven't understood yet.");

	strQuote[1] = str_create("USER: This is a known bug.
JCL: It was on the bug list, but quite down below.");

	strQuote[2] = str_create("USER: Wer checkt diese Shaderprogrammierung denn?
JCL: Kein Mensch zwingt Sie jedoch, mit Shadern oder ueberhaupt mit Gamestudio zu arbeiten. Es gibt schliesslich auch andere schoene Hobbies, wie zum Beispiel das Sammeln von Bierdeckeln.");

	strQuote[3] = str_create("JCL: This is a known problem. It's _not_ due to the camera inside the volume, but due to the precalculation whether a shadow shall be visible or not (for frame rate reasons). This precalculation sometimes fails and we'll replace it in a future update by a simpler method. At the moment there's no workaround.");

	strQuote[4] = str_create("USER: Hi.... can you make a better shadow? because the shadow haves a problems.
JCL: I think we won't replace Z buffer shadows. We'll need them as punishment for people who don't read the manual about how to do better shadows.");

	strQuote[5] = str_create("USER: Da es im Forecast auf Medium steht, kann es evtl. sogar noch Ewigkeiten dauern bzw. auch ganz wegfallen. Mach also lieber keine großen Planungen damit für ein Projekt.
JCL: Es, we won't give a date for features.");

	strQuote[6] = str_create("USER: The copy and paste editing tools don't appear to be working in SED editor.. Or is it just a problem my end..
JCL: I guess on your end.");

	strQuote[7] = str_create("JCL: Wenn ich Populist wäre, müsste ich mich jetzt sofort hinsetzen und das Feature programmieren. Gut, dass ich kein Populist bin.");

	strQuote[8] = str_create("JCL: 'Lag' is a normally a notion from network games and means the time between input and reaction due to the message travel time. So I'm not sure what you're talking about here. Maybe bad frame rate? Your 3D card is fine and the engine has no such thing as lag. If you suffer from lag, I recommend doing some sports, such as table tennis.");

	strQuote[9] = str_create("USER: If i set fps_max to 30 the movement isnt smooth anymore. How to make it smooth on 30 fps? p.s. On 60 fps it works fine.
JCL: I can officially confirm that 30 fps is indeed half as smooth as 60 fps - if that was your question.");

	strQuote[10] = str_create("JCL: Probably the secret of the high frame rate of professional games like HL2 is that they don't permanently move all entities up and down.");

	strQuote[11] = str_create("JCL: We want to maintain a clean, decent, American family suited forum look... which means you may post zombies or chainsaw massacres, but no erotic.");

	strQuote[12] = str_create("JCL: When you leave the protected room of engine functions and wander into the open field of the Windows API, don't complain about the cold wind of real programming blowing into your face.");

	strQuote[13] = str_create("JCL: When you want to post something into a discussion, it might be a good idea to understand the issue.");

	strQuote[14] = str_create("USER: The undocumented UNTOUCHABLE flag for panels won't work.
JCL: That's why it's undocumented I guess.");

	strQuote[15] = str_create("JCL: Judging from the compiler source, macro stringification seems to be already implemented but not to work. As soon as I've understood the Chinese comments in the source, I have a chance to make it work.");

	strQuote[16] = str_create("USER: I get an editor warning error message 'WARNING W091 Region disaligned'. There is NO mention of what it is or how to solve it in the manual.
JCL: I admit that I don't know this either.");

	strQuote[17] = str_create("MICHAEL: Where did the 'Show Normals' button go?
JCL: I believe the 'Show Normals' button took his leave about 2003, and is now enjoying his retirement at the beach in Acapulco.");

	strQuote[18] = str_create("USER: Is there ANY hope that bmap_save can be overhauled [...] so that it is capable of saving NON-Power-of-two sizes?
JCL: I'm not making those rules. But if you promise not to tell, I'll check if we can step around them.");

	strQuote[19] = str_create("USER: Silence When I try to add a sound to my level in WED all's it says is empty.
JCL: Hmm - maybe you could tell a little more detailed what your problem is. Or just try recording a sound that says: 'It's full'.");

	strQuote[20] = str_create("JCL: Hmm, I don't know either what this error message means.");

	strQuote[21] = str_create("USER: [...] my question is what could cause dplay_id to not be sent/updated property?
JCL: An earthquake, possibly.");

	txtQuote = txt_create(1, 16);
	txtQuote->font = fntQuote;
	txtQuote->flags |= WWRAP | OUTLINE;
	txtQuote->pos_y = 70;
	text_outline = 150;
}

void remove_quotes()
{
	var i;
	
	for (i = 0; i < NUMQUOTES; i++)
	{
		str_remove(strQuote[i]);
	}
}
