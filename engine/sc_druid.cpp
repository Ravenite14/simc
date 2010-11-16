// ==========================================================================
// Dedmonwakeen's DPS-DPM Simulator.
// Send questions to natehieter@gmail.com
// ==========================================================================

#include "simulationcraft.h"

// ==========================================================================
// Druid
// ==========================================================================

struct druid_t : public player_t
{
  // Active
  action_t* active_t10_4pc_caster_dot;

  // Auto-attacks
  attack_t* cat_melee_attack;
  attack_t* bear_melee_attack;

  double equipped_weapon_dps;

  // Buffs
  buff_t* buffs_berserk;
  buff_t* buffs_bear_form;
  buff_t* buffs_cat_form;
  buff_t* buffs_combo_points;
  buff_t* buffs_eclipse_lunar;
  buff_t* buffs_eclipse_solar;
  buff_t* buffs_enrage;
  buff_t* buffs_glyph_of_innervate;
  buff_t* buffs_lacerate;
  buff_t* buffs_lunar_shower;
  buff_t* buffs_moonkin_form;
  buff_t* buffs_natures_grace;
  buff_t* buffs_natures_swiftness;
  buff_t* buffs_omen_of_clarity;
  buff_t* buffs_pulverize;
  buff_t* buffs_savage_roar;
  buff_t* buffs_shooting_stars;
  buff_t* buffs_stealthed;
  buff_t* buffs_t8_4pc_caster;
  buff_t* buffs_t10_2pc_caster;
  buff_t* buffs_t11_4pc_caster;
  buff_t* buffs_tigers_fury;

  // Cooldowns
  cooldown_t* cooldowns_mangle_bear;
  cooldown_t* cooldowns_fury_swipes;

  // DoTs
  dot_t* dots_rip;
  dot_t* dots_rake;
  dot_t* dots_insect_swarm;
  dot_t* dots_moonfire;

  // Gains
  gain_t* gains_bear_melee;
  gain_t* gains_energy_refund;
  gain_t* gains_enrage;
  gain_t* gains_euphoria;
  gain_t* gains_glyph_of_innervate;
  gain_t* gains_incoming_damage;
  gain_t* gains_moonkin_form;
  gain_t* gains_natural_reaction;
  gain_t* gains_omen_of_clarity;
  gain_t* gains_primal_fury;
  gain_t* gains_tigers_fury;

  // Procs
  proc_t* procs_combo_points_wasted;
  proc_t* procs_fury_swipes;
  proc_t* procs_parry_haste;
  proc_t* procs_primal_fury;
  proc_t* procs_tier8_2pc;

  // Random Number Generation
  rng_t* rng_euphoria;
  rng_t* rng_fury_swipes;
  rng_t* rng_blood_in_the_water;
  rng_t* rng_primal_fury;
  
  // Tree specialization passives
  // Balance
  passive_spell_t* spec_moonfury;
  mastery_t* mastery_total_eclipse; // Mastery
  double eclipse_bar_value; // Tracking the current value of the eclipse bar
  int    eclipse_bar_direction; // Tracking the current direction of the eclipse bar
  
  // Feral
  passive_spell_t* spec_aggression;
  passive_spell_t* spec_vengeance;
  mastery_t* mastery_razor_claws; // Mastery
  mastery_t* mastery_savage_defender; // Mastery

  // Up-Times
  uptime_t* uptimes_energy_cap;
  uptime_t* uptimes_rage_cap;
  uptime_t* uptimes_rip;
  uptime_t* uptimes_rake;


  // Talents
  struct talents_t 
  {
    // Balance
    talent_t* balance_of_power;
    talent_t* earth_and_moon;
    talent_t* euphoria;
    talent_t* dreamstate;
    talent_t* force_of_nature;
    talent_t* fungal_growth;
    talent_t* gale_winds;
    talent_t* genesis;
    talent_t* lunar_shower;
    talent_t* moonglow;
    talent_t* moonkin_form;
    talent_t* natures_grace;
    talent_t* natures_majesty;
//    talent_t* overgrowth;
    talent_t* owlkin_frenzy;
    talent_t* shooting_stars;
    talent_t* solar_beam;
    talent_t* starfall;
    talent_t* starlight_wrath;
    talent_t* sunfire;
    talent_t* typhoon;
    
    // Feral  
    talent_t* berserk;
    talent_t* blood_in_the_water;
    talent_t* brutal_impact; // NYI
    talent_t* endless_carnage;
    talent_t* feral_aggression;
    talent_t* feral_charge; // NYI
    talent_t* feral_swiftness;
    talent_t* furor;
    talent_t* fury_swipes;
    talent_t* infected_wounds;
    talent_t* king_of_the_jungle;
    talent_t* leader_of_the_pack;
    talent_t* natural_reaction;
    talent_t* nurturing_instict; // NYI
    talent_t* predatory_strikes; // NYI
    talent_t* primal_fury;
    talent_t* primal_madness; // NYI
    talent_t* pulverize; // NYI
    talent_t* rend_and_tear;
    talent_t* stampede; // Valid to run out->charge->ravage?
    talent_t* survival_instincts; // NYI
    talent_t* thick_hide; // How does the +10% and +33%@bear stack etc

    
    // Resto
    talent_t* blessing_of_the_grove;
    talent_t* fury_of_stormrage; // NYI
    talent_t* heart_of_the_wild;
    talent_t* master_shapeshifter;
    talent_t* natures_swiftness;

    talents_t() { memset( ( void* ) this, 0x0, sizeof( talents_t ) ); }
 	};
 	talents_t talents;

  struct glyphs_t
  {
    int berserk;
    int ferocious_bite;
    int focus;
    int innervate;
    int insect_swarm;
    int lacerate;
    int mangle;
    int monsoon;
    int moonfire;
    int rip;
    int savage_roar;
    int shred;
    int starfall;
    int starfire;
    int starsurge;
    int tigers_fury;
    int typhoon;
    int wrath;
    glyphs_t() { memset( ( void* ) this, 0x0, sizeof( glyphs_t ) ); }
  };
  glyphs_t glyphs;

  druid_t( sim_t* sim, const std::string& name, race_type r = RACE_NONE ) : player_t( sim, DRUID, name, r )
  {
    tree_type[ DRUID_BALANCE     ] = TREE_BALANCE;
    tree_type[ DRUID_FERAL       ] = TREE_FERAL;
    tree_type[ DRUID_RESTORATION ] = TREE_RESTORATION;

    active_t10_4pc_caster_dot = 0;
    
    eclipse_bar_value     = 0;
    eclipse_bar_direction = 0;

    cooldowns_mangle_bear = get_cooldown( "mangle_bear" );
    cooldowns_fury_swipes = get_cooldown( "fury_swipes" );

    distance = 30;

    dots_rip          = get_dot( "rip" );
    dots_rake         = get_dot( "rake" );
    dots_insect_swarm = get_dot( "insect_swarm" );
    dots_moonfire     = get_dot( "moonfire" );

    cat_melee_attack = 0;
    bear_melee_attack = 0;

    equipped_weapon_dps = 0; 
  }

  // Character Definition
  virtual void      init_talents();
  virtual void      init_spells();
  virtual void      init_glyphs();
  virtual void      init_race();
  virtual void      init_base();
  virtual void      init_buffs();
  virtual void      init_items();
  virtual void      init_scaling();
  virtual void      init_gains();
  virtual void      init_procs();
  virtual void      init_uptimes();
  virtual void      init_rng();
  virtual void      init_actions();
  virtual void      reset();
  virtual void      interrupt();
  virtual void      clear_debuffs();
  virtual void      regen( double periodicity );
  virtual double    available() SC_CONST;
  virtual double    composite_attack_power() SC_CONST;
  virtual double    composite_attack_power_multiplier() SC_CONST;
  virtual double    composite_attack_crit() SC_CONST;
  virtual double    composite_spell_hit() SC_CONST;
  virtual double    composite_spell_crit() SC_CONST;
  virtual double    composite_attribute_multiplier( int attr ) SC_CONST;
  virtual double    matching_gear_multiplier( const attribute_type attr ) SC_CONST;
  virtual double    composite_block_value() SC_CONST { return 0; }
  virtual double    composite_tank_parry() SC_CONST { return 0; }
  virtual double    composite_tank_block() SC_CONST { return 0; }
  virtual double    composite_tank_crit( const school_type school ) SC_CONST;
  virtual action_expr_t* create_expression( action_t*, const std::string& name );
  virtual std::vector<talent_translation_t>& get_talent_list();
  virtual std::vector<option_t>& get_options();
  virtual action_t* create_action( const std::string& name, const std::string& options );
  virtual pet_t*    create_pet( const std::string& name );
  virtual void      create_pets();
  virtual int       decode_set( item_t& item );
  virtual int       primary_resource() SC_CONST;
  virtual int       primary_role() SC_CONST;
  virtual int       target_swing();

  // Utilities
  double combo_point_rank( double* cp_list ) SC_CONST
  {
    assert( buffs_combo_points -> check() );
    return cp_list[ buffs_combo_points -> stack() - 1 ];
  }
  double combo_point_rank( double cp1, double cp2, double cp3, double cp4, double cp5 ) SC_CONST
  {
    double cp_list[] = { cp1, cp2, cp3, cp4, cp5 };
    return combo_point_rank( cp_list );
  }
  void reset_gcd()
  {
    for ( action_t* a=action_list; a; a = a -> next )
    {
      if ( a -> trigger_gcd != 0 ) a -> trigger_gcd = base_gcd;
    }
  }
};

namespace { // ANONYMOUS NAMESPACE ==========================================

// ==========================================================================
// Druid Cat Attack
// ==========================================================================

struct druid_cat_attack_t : public attack_t
{
  int requires_stealth;
  int requires_position;
  bool requires_combo_points;
  bool adds_combo_points;
  int berserk, min_combo_points, max_combo_points;
  double min_energy, max_energy;
  double min_mangle_expire, max_mangle_expire;
  double min_savage_roar_expire, max_savage_roar_expire;
  double min_rip_expire, max_rip_expire;
  double min_rake_expire, max_rake_expire;

  druid_cat_attack_t( const char* n, player_t* player, const school_type s=SCHOOL_PHYSICAL, int t=TREE_NONE, bool special=true ) :
      attack_t( n, player, RESOURCE_ENERGY, s, t, special ),
      requires_stealth( 0 ),
      requires_position( POSITION_NONE ),
      requires_combo_points( false ),
      adds_combo_points( false ),
      berserk( 0 ),
      min_combo_points( 0 ), max_combo_points( 0 ),
      min_energy( 0 ), max_energy( 0 ),
      min_mangle_expire( 0 ), max_mangle_expire( 0 ),
      min_savage_roar_expire( 0 ), max_savage_roar_expire( 0 ),
      min_rip_expire( 0 ), max_rip_expire( 0 ),
      min_rake_expire( 0 ), max_rake_expire( 0 )
  {
    tick_may_crit = true;
  }

  virtual void   parse_options( option_t*, const std::string& options_str );
  virtual double cost() SC_CONST;
  virtual void   execute();
  virtual void   consume_resource();
  virtual void   player_buff();
  virtual bool   ready();
};

// ==========================================================================
// Druid Bear Attack
// ==========================================================================

struct druid_bear_attack_t : public attack_t
{
  int berserk;
  double min_rage, max_rage;
  double min_mangle_expire, max_mangle_expire;
  double min_lacerate_expire, max_lacerate_expire;
  int    min_lacerate_stack, max_lacerate_stack;

  druid_bear_attack_t( const char* n, player_t* player, const school_type s=SCHOOL_PHYSICAL, int t=TREE_NONE, bool special=true ) :
      attack_t( n, player, RESOURCE_RAGE, s, t, true ),
      berserk( 0 ),
      min_rage( 0 ), max_rage( 0 ),
      min_mangle_expire( 0 ), max_mangle_expire( 0 ),
      min_lacerate_expire( 0 ), max_lacerate_expire( 0 ),
      min_lacerate_stack( 0 ), max_lacerate_stack( 0 )
  {
  }

  virtual void   parse_options( option_t*, const std::string& options_str );
  virtual double cost() SC_CONST;
  virtual void   execute();
  virtual void   consume_resource();
  virtual void   player_buff();
  virtual bool   ready();
};

// ==========================================================================
// Druid Spell
// ==========================================================================

struct druid_spell_t : public spell_t
{
  int    skip_on_eclipse;
  double base_cost_reduction;
  druid_spell_t( const char* n, player_t* p, const school_type s, int t ) :
      spell_t( n, p, RESOURCE_MANA, s, t ), skip_on_eclipse( 0 ),
      base_cost_reduction( 0.0 )
      { }
  virtual void   consume_resource();
  virtual double cost() SC_CONST;
  virtual double cost_reduction() SC_CONST;
  virtual void   execute();
  virtual double execute_time() SC_CONST;
  virtual double haste() SC_CONST;
  virtual void   parse_options( option_t*, const std::string& options_str );
  virtual void   player_buff();
  virtual bool   ready();
  virtual void   schedule_execute();
  virtual void   target_debuff( int dmg_type );
};

// ==========================================================================
// Pet Treants
// ==========================================================================

struct treants_pet_t : public pet_t
{
  struct melee_t : public attack_t
  {
    melee_t( player_t* player ) :
        attack_t( "treant_melee", player )
    {
      //druid_t* o = player -> cast_pet() -> owner -> cast_druid();

      weapon = &( player -> main_hand_weapon );
      base_execute_time = weapon -> swing_time;
      base_dd_min = base_dd_max = 1;
      background = true;
      repeating = true;
      may_crit = true;

      // Model the three Treants as one actor hitting 3x hard
      base_multiplier *= 3.0;
    }
  };

  melee_t* melee;

  treants_pet_t( sim_t* sim, player_t* owner, const std::string& pet_name ) :
      pet_t( sim, owner, pet_name ), melee( 0 )
  {
    main_hand_weapon.type       = WEAPON_BEAST;
    main_hand_weapon.min_dmg    = 340;
    main_hand_weapon.max_dmg    = 340;
    main_hand_weapon.damage     = ( main_hand_weapon.min_dmg + main_hand_weapon.max_dmg ) / 2;
    main_hand_weapon.swing_time = 1.8;
  }
  virtual void init_base()
  {
    pet_t::init_base();

    attribute_base[ ATTR_STRENGTH  ] = 331;
    attribute_base[ ATTR_AGILITY   ] = 113;
    attribute_base[ ATTR_STAMINA   ] = 361;
    attribute_base[ ATTR_INTELLECT ] = 65;
    attribute_base[ ATTR_SPIRIT    ] = 109;

    base_attack_power = -20;
    initial_attack_power_per_strength = 2.0;

    melee = new melee_t( this );
  }
  virtual double composite_attack_power() SC_CONST
  {
    double ap = pet_t::composite_attack_power();
    ap += 0.57 * owner -> composite_spell_power( SCHOOL_MAX );
    return ap;
  }
  virtual double composite_attack_hit() SC_CONST
  {
    return owner -> composite_spell_hit();
  }
  virtual double composite_attack_expertise() SC_CONST
  {
    return owner -> composite_spell_hit() * 26.0 / 17.0;
  }
  virtual void schedule_ready( double delta_time=0,
                               bool   waiting=false )
  {
    pet_t::schedule_ready( delta_time, waiting );
    if ( ! melee -> execute_event ) melee -> execute();
  }
  virtual void interrupt()
  {
    pet_t::interrupt();
    melee -> cancel();
  }
};

// add_combo_point =========================================================

static void add_combo_point( druid_t* p )
{
  if ( p -> buffs_combo_points -> current_stack == 5 )
  {
    p -> procs_combo_points_wasted -> occur();
    return;
  }

  p -> buffs_combo_points -> trigger();
}

// trigger_omen_of_clarity ==================================================

static void trigger_omen_of_clarity( action_t* a )
{
  druid_t* p = a -> player -> cast_druid();

  if ( a -> aoe ) return;
  if ( a -> proc ) return;

  if ( p -> buffs_omen_of_clarity -> trigger() )
    p -> buffs_t10_2pc_caster -> trigger( 1, 0.15 );

}

// trigger_fury_swipes ======================================================

static void trigger_fury_swipes( action_t* a )
{
  druid_t* p = a -> player -> cast_druid();

  if ( a -> proc ) return;

  if ( a -> result_is_miss() ) return;

  if ( ! a -> weapon ) return;

  if ( ! p -> talents.fury_swipes -> rank() )
    return;

  if ( p -> cooldowns_fury_swipes -> remains() > 0 )
    return;

  if ( p -> rng_fury_swipes -> roll( 0.04 * p -> talents.fury_swipes -> rank() ) )
  {
    if ( a -> sim -> log )
      log_t::output( a -> sim, "%s gains one extra attack through %s",
                     p -> name(), p -> procs_fury_swipes -> name() );

    p -> procs_fury_swipes -> occur();
    p -> cooldowns_fury_swipes -> start( 3.0 );

    p -> main_hand_attack -> proc = true;
    p -> main_hand_attack -> execute();
    p -> main_hand_attack -> proc = false;
  }
}

// trigger_infected_wounds ==================================================

static void trigger_infected_wounds( action_t* a )
{
  druid_t* p = a -> player -> cast_druid();

  if ( p -> talents.infected_wounds -> rank() )
  {
    a -> target -> debuffs.infected_wounds -> trigger();
    a -> target -> debuffs.infected_wounds -> source = p;
  }
}

// trigger_earth_and_moon ===================================================

static void trigger_earth_and_moon( spell_t* s )
{
  druid_t* p = s -> player -> cast_druid();

  if ( p -> talents.earth_and_moon -> rank() == 0 ) return;

  target_t* t = s -> target;

  t -> debuffs.earth_and_moon -> trigger( 1, 8 );
  t -> debuffs.earth_and_moon -> source = p;
}

// trigger_eclipse_proc ====================================================

static void trigger_eclipse_proc( druid_t* p )
{
  // All extra procs when eclipse pops
  p -> resource_gain( RESOURCE_MANA, p -> resource_max[ RESOURCE_MANA ] * 0.01 * p -> talents.euphoria -> effect_base_value( 3 ) , p -> gains_euphoria );
  p -> buffs_t11_4pc_caster -> trigger( 3 );
  p -> buffs_natures_grace -> cooldown -> reset();
  
  // When eclipse procs the direction of the bar switches!
  p -> eclipse_bar_direction = - p -> eclipse_bar_direction;
}

// trigger_eclipse_energy_gain ==============================================

static void trigger_eclipse_energy_gain( spell_t* s, double gain )
{
  if ( gain == 0 ) return;
  
  druid_t* p = s -> player -> cast_druid();

  if ( p -> eclipse_bar_direction == 0 )
  {
    // No eclipse gained at all by now => Start of the encounter
    // Set the direction to the direction of the first gain!
    p -> eclipse_bar_direction = ( gain > 0 ? 1 : -1 );
  }
  else
  {
    // If the gain isn't not alligned with the direction of the bar it won't happen
    // +*+ == -*- => >0 => fine
    // -*+ == +*- => <0 => Not alligned, no gain
    if ( p -> eclipse_bar_direction * gain < 0 )
      return;
  }

  double old_eclipse_bar_value = p -> eclipse_bar_value;
  p -> eclipse_bar_value += gain;

  if ( p -> eclipse_bar_value < 0 ) 
  {
    p -> buffs_eclipse_solar -> expire();

    if ( p -> eclipse_bar_value < -100 ) 
      p -> eclipse_bar_value = -100;
  }

  if ( p -> eclipse_bar_value > 0 ) 
  {
    p -> buffs_eclipse_lunar -> expire();

    if ( p -> eclipse_bar_value > 100 ) 
      p -> eclipse_bar_value = 100;
  }
  
  double actual_gain = p -> eclipse_bar_value - old_eclipse_bar_value;
  if ( s -> sim -> log )
  {
    log_t::output( s -> sim, "%s gains %.2f (%.2f) %s from %s (%.2f)",
                   p -> name(), actual_gain, gain,
                   "Eclipse", s -> name(),
                   p -> eclipse_bar_value );
  }
  
  
  // Eclipse proc:
  // Procs when you reach 100 and only then, you have to have an
  // actual gain of eclipse energy (bar value)
  if ( actual_gain != 0 )
  {
    if ( p -> eclipse_bar_value == 100 ) 
    {
      if ( p -> buffs_eclipse_solar -> trigger() )
        trigger_eclipse_proc( p );
    }
    else if ( p -> eclipse_bar_value == -100 ) 
    {
      if ( p -> buffs_eclipse_lunar -> trigger() )
        trigger_eclipse_proc( p );
    }
  }
}

// trigger_t8_2pc_melee ====================================================

static void trigger_t8_2pc_melee( action_t* a )
{
  druid_t* p = a -> player -> cast_druid();

  if ( ! p -> set_bonus.tier8_2pc_melee() )
    return;

  /* 2% chance on tick, http://ptr.wowhead.com/?spell=64752
  /  It is just another chance to procc the same clearcasting
  /  buff that OoC provides. OoC and t8_2pc_melee overwrite
  /  each other. Easier to treat it just (ab)use the OoC handling */

  if ( p -> buffs_omen_of_clarity -> trigger( 1, 1, 0.02 ) )
    p -> procs_tier8_2pc -> occur();
}

// trigger_t10_4pc_caster ==================================================

static void trigger_t10_4pc_caster( player_t* player, double direct_dmg, int school )
{
  druid_t* p = player -> cast_druid();

  if ( ! p -> set_bonus.tier10_4pc_caster() ) return;

  struct t10_4pc_caster_dot_t : public druid_spell_t
  {
    t10_4pc_caster_dot_t( player_t* player ) : druid_spell_t( "tier10_4pc_balance", player, SCHOOL_NATURE, TREE_BALANCE )
    {
      may_miss         = false;
      may_resist       = false;
      background       = true;
      proc             = true;
      trigger_gcd      = 0;
      base_cost        = 0;
      base_multiplier  = 1.0;
      tick_power_mod   = 0;
      base_tick_time   = 2.0;
      num_ticks        = 2;
      number_ticks     = num_ticks;
      scale_with_haste = false;
      id               = 71023;
      reset();
    }
    void player_buff() {}
    void target_debuff( int dmg_type ) {}
  };

  if ( ! p -> active_t10_4pc_caster_dot ) p -> active_t10_4pc_caster_dot = new t10_4pc_caster_dot_t( p );

  double dmg = direct_dmg * 0.07;
  if (  p -> active_t10_4pc_caster_dot -> ticking )
  {
    int num_ticks =  p -> active_t10_4pc_caster_dot -> num_ticks;
    int remaining_ticks = num_ticks -  p -> active_t10_4pc_caster_dot -> current_tick;

    dmg +=  p -> active_t10_4pc_caster_dot -> base_td * remaining_ticks;

    p -> active_t10_4pc_caster_dot -> cancel();
  }
   p -> active_t10_4pc_caster_dot -> base_td = dmg /  p -> active_t10_4pc_caster_dot -> num_ticks;
   p -> active_t10_4pc_caster_dot -> schedule_tick();
}

// trigger_primal_fury =====================================================

static void trigger_primal_fury( druid_cat_attack_t* a )
{
  druid_t* p = a -> player -> cast_druid();

  if ( ! p -> talents.primal_fury -> rank() )
    return;

  if ( ! a -> adds_combo_points )
    return;

  if ( p -> rng_primal_fury -> roll( p -> talents.primal_fury -> rank() * 0.5 ) )
  {
    add_combo_point( p );
    p -> procs_primal_fury -> occur();
  }
}

// trigger_energy_refund ===================================================

static void trigger_energy_refund( druid_cat_attack_t* a )
{
  druid_t* p = a -> player -> cast_druid();

  if ( ! a -> adds_combo_points )
    return;

  double energy_restored = a -> resource_consumed * 0.80;

  p -> resource_gain( RESOURCE_ENERGY, energy_restored, p -> gains_energy_refund );
}

// trigger_primal_fury =====================================================

static void trigger_primal_fury( druid_bear_attack_t* a )
{
  druid_t* p = a -> player -> cast_druid();

  if ( ! p -> talents.primal_fury -> rank() )
    return;

  if ( p -> rng_primal_fury -> roll( p -> talents.primal_fury -> rank() * 0.5 ) )
  {
    p -> resource_gain( RESOURCE_RAGE, 5.0, p -> gains_primal_fury );
  }
}

// trigger_rage_gain =======================================================

static void trigger_rage_gain( druid_bear_attack_t* a )
{
  druid_t* p = a -> player -> cast_druid();

  double rage_gain = 0;

  p -> resource_gain( RESOURCE_RAGE, rage_gain, p -> gains_bear_melee );
}

// =========================================================================
// Druid Attack
// =========================================================================

// druid_cat_attack_t::parse_options =======================================

void druid_cat_attack_t::parse_options( option_t*          options,
                                        const std::string& options_str )
{
  option_t base_options[] =
  {
    { "berserk",          OPT_INT,  &berserk                },
    { "min_combo_points", OPT_INT,  &min_combo_points       },
    { "max_combo_points", OPT_INT,  &max_combo_points       },
    { "cp>",              OPT_INT,  &min_combo_points       },
    { "cp<",              OPT_INT,  &max_combo_points       },
    { "min_energy",       OPT_FLT,  &min_energy             },
    { "max_energy",       OPT_FLT,  &max_energy             },
    { "energy>",          OPT_FLT,  &min_energy             },
    { "energy<",          OPT_FLT,  &max_energy             },
    { "rip>",             OPT_FLT,  &min_rip_expire         },
    { "rip<",             OPT_FLT,  &max_rip_expire         },
    { "rake>",            OPT_FLT,  &min_rake_expire        },
    { "rake<",            OPT_FLT,  &max_rake_expire        },
    { "mangle>",          OPT_FLT,  &min_mangle_expire      },
    { "mangle<",          OPT_FLT,  &max_mangle_expire      },
    { "savage_roar>",     OPT_FLT,  &min_savage_roar_expire },
    { "savage_roar<",     OPT_FLT,  &max_savage_roar_expire },
    { NULL, OPT_UNKNOWN, NULL }
  };
  std::vector<option_t> merged_options;
  attack_t::parse_options( merge_options( merged_options, options, base_options ), options_str );
}

// druid_cat_attack_t::cost ================================================

double druid_cat_attack_t::cost() SC_CONST
{
  druid_t* p = player -> cast_druid();
  double c = attack_t::cost();
  if ( c == 0 ) return 0;
  if ( harmful &&  p -> buffs_omen_of_clarity -> check() ) return 0;
  if ( p -> buffs_berserk -> check() ) c *= 0.5;
  return c;
}

// druid_cat_attack_t::consume_resource ====================================

void druid_cat_attack_t::consume_resource()
{
  druid_t* p = player -> cast_druid();
  attack_t::consume_resource();
  if ( harmful && p -> buffs_omen_of_clarity -> up() )
  {
    // Treat the savings like a energy gain.
    double amount = attack_t::cost();
    if ( amount > 0 )
    {
      p -> gains_omen_of_clarity -> add( amount );
      p -> buffs_omen_of_clarity -> expire();
    }
  }
}

// druid_cat_attack_t::execute =============================================

void druid_cat_attack_t::execute()
{
  druid_t* p = player -> cast_druid();

  attack_t::execute();

  if ( result_is_hit() )
  {
    if ( requires_combo_points ) p -> buffs_combo_points -> expire();
    if (     adds_combo_points ) add_combo_point ( p );

    if ( result == RESULT_CRIT )
    {
      trigger_primal_fury( this );
    }
  }
  else
  {
    trigger_energy_refund( this );
  }

  if ( harmful ) p -> buffs_stealthed -> expire();
}

// druid_cat_attack_t::player_buff =========================================

void druid_cat_attack_t::player_buff()
{
  druid_t* p = player -> cast_druid();

  attack_t::player_buff();

  player_multiplier *= 1.0 + p -> buffs_tigers_fury -> value();
  if (school == SCHOOL_BLEED)
    player_multiplier *= 1.0 + 0.0313 * p->composite_mastery();

  p -> uptimes_rip  -> update( p -> dots_rip  -> ticking() );
  p -> uptimes_rake -> update( p -> dots_rake -> ticking() );
}

// druid_cat_attack_t::ready ===============================================

bool druid_cat_attack_t::ready()
{
  if ( ! attack_t::ready() )
    return false;

  druid_t*  p = player -> cast_druid();
  target_t* t = target;

  if ( ! p -> buffs_cat_form -> check() )
    return false;

  if ( requires_position != POSITION_NONE )
    if ( p -> position != requires_position )
      return false;

  if ( requires_stealth )
    if ( ! p -> buffs_stealthed -> check() )
      return false;

  if ( requires_combo_points && ! p -> buffs_combo_points -> check() )
    return false;

  if ( berserk && ! p -> buffs_berserk -> check() )
    return false;

  if ( min_combo_points > 0 )
    if ( p -> buffs_combo_points -> current_stack < min_combo_points )
      return false;

  if ( max_combo_points > 0 )
    if ( p -> buffs_combo_points -> current_stack > max_combo_points )
      return false;

  if ( min_energy > 0 )
    if ( p -> resource_current[ RESOURCE_ENERGY ] < min_energy )
      return false;

  if ( max_energy > 0 )
    if ( p -> resource_current[ RESOURCE_ENERGY ] > max_energy )
      return false;

  double ct = sim -> current_time;

  if ( min_mangle_expire > 0 )
    if ( t -> debuffs.mangle -> remains_lt( min_mangle_expire ) )
      return false;

  if ( max_mangle_expire > 0 )
    if ( t -> debuffs.mangle -> remains_gt( max_mangle_expire ) )
      return false;

  if ( min_savage_roar_expire > 0 )
    if ( p -> buffs_savage_roar -> remains_lt( min_savage_roar_expire ) )
      return false;

  if ( max_savage_roar_expire > 0 )
    if ( p -> buffs_savage_roar -> remains_gt( max_savage_roar_expire ) )
      return false;

  if ( min_rip_expire > 0 )
    if ( ! p -> dots_rip -> ticking() || ( ( p -> dots_rip -> ready - ct ) < min_rip_expire ) )
      return false;

  if ( max_rip_expire > 0 )
    if ( p -> dots_rip -> ticking() && ( ( p -> dots_rip -> ready - ct ) > max_rip_expire ) )
      return false;

  if ( min_rake_expire > 0 )
    if ( ! p -> dots_rake -> ticking() || ( ( p -> dots_rake -> ready - ct ) < min_rake_expire ) )
      return false;

  if ( max_rake_expire > 0 )
    if ( p -> dots_rake -> ticking() && ( ( p -> dots_rake -> ready - ct ) > max_rake_expire ) )
      return false;

  return true;
}

// Cat Melee Attack ========================================================

struct cat_melee_t : public druid_cat_attack_t
{
  cat_melee_t( player_t* player ) :
    druid_cat_attack_t( "cat_melee", player, SCHOOL_PHYSICAL, TREE_NONE, /*special*/false )
  {
    base_dd_min = base_dd_max = 1;
    background  = true;
    repeating   = true;
    trigger_gcd = 0;
    base_cost   = 0;
    may_crit    = true;
  }
  virtual void player_buff()
  {
    druid_cat_attack_t::player_buff();
    druid_t* p = player -> cast_druid();
    player_multiplier *= 1 + p -> buffs_savage_roar -> value();
  }

  virtual double execute_time() SC_CONST
  {
    if ( ! player -> in_combat ) return 0.01;
    return druid_cat_attack_t::execute_time();
  }

  virtual void execute()
  {
    druid_cat_attack_t::execute();
    if ( result_is_hit() )
    {
      trigger_fury_swipes( this );
      trigger_omen_of_clarity( this );
    }
  }
};

// Berserk =================================================================

struct berserk_cat_t : public druid_cat_attack_t
{
  int tigers_fury;

  berserk_cat_t( player_t* player, const std::string& options_str ) :
    druid_cat_attack_t( "berserk_cat", player ),
    tigers_fury( 0 )
  {
    druid_t* p = player -> cast_druid();
    check_talent( p -> talents.berserk -> rank() );

    option_t options[] =
    {
      { "tigers_fury", OPT_BOOL, &tigers_fury },
      { NULL, OPT_UNKNOWN, NULL }
    };

    parse_options( options, options_str );

    cooldown = p -> get_cooldown( "berserk" );
    cooldown -> duration = 180;
    id = 50334;
  }

  virtual void execute()
  {
    druid_t* p = player -> cast_druid();
    if ( sim -> log ) log_t::output( sim, "%s performs %s", p -> name(), name() );
    // Berserk cancels TF
    p -> buffs_tigers_fury -> expire();
    p -> buffs_berserk -> trigger();
    update_ready();
  }

  virtual bool ready()
  {
    druid_t* p = player -> cast_druid();
    if ( tigers_fury && ! p -> buffs_tigers_fury -> check() )
      return false;
    return druid_cat_attack_t::ready();
  }
};

// Claw ====================================================================

struct claw_t : public druid_cat_attack_t
{
  claw_t( player_t* player, const std::string& options_str ) :
    druid_cat_attack_t( "claw", player, SCHOOL_PHYSICAL, TREE_FERAL )
  {
    druid_t* p = player -> cast_druid();

    option_t options[] =
    {
      { NULL, OPT_UNKNOWN, NULL }
    };
    parse_options( options, options_str );

    id = 1082;
    parse_data( p -> player_data );

    weapon = &( p -> main_hand_weapon );
    adds_combo_points = true;
    may_crit          = true;
  }
};

// Maim ======================================================================

struct maim_t : public druid_cat_attack_t
{
  maim_t( player_t* player, const std::string& options_str ) :
    druid_cat_attack_t( "maim", player, SCHOOL_PHYSICAL, TREE_FERAL )
  {
    druid_t* p = player -> cast_druid();

    option_t options[] =
    {
      { NULL, OPT_UNKNOWN, NULL }
    };
    parse_options( options, options_str );

    id = 22570;
    parse_data( p -> player_data );
    
    weapon = &( p -> main_hand_weapon );
    weapon_power_mod = 0;

    may_crit = true;
    requires_combo_points = true;
  }

  virtual bool ready()
  {
    if ( ! target -> debuffs.casting -> check() ) return false;
    return druid_cat_attack_t::ready();
  }
};

// Mangle (Cat) ============================================================

struct mangle_cat_t : public druid_cat_attack_t
{
  mangle_cat_t( player_t* player, const std::string& options_str ) :
      druid_cat_attack_t( "mangle_cat", player, SCHOOL_PHYSICAL, TREE_FERAL )
  {
    druid_t* p = player -> cast_druid();

    // By default, do not overwrite Mangle
    max_mangle_expire = 0.001;

    option_t options[] =
    {
      { NULL, OPT_UNKNOWN, NULL }
    };
    parse_options( options, options_str );

    id = 33876;
    parse_data( p -> player_data );

    adds_combo_points = true;
    may_crit          = true;

    base_multiplier  *= 1.0 + p -> glyphs.mangle * 0.1;
  }

  virtual void execute()
  {
    druid_cat_attack_t::execute();
    if ( result_is_hit() )
    {
      druid_t* p = player -> cast_druid();
      target -> debuffs.mangle -> trigger();
      target -> debuffs.mangle -> source = p;
      trigger_infected_wounds( this );
    }
  }
};

// Rake ====================================================================

struct rake_t : public druid_cat_attack_t
{
  rake_t( player_t* player, const std::string& options_str ) :
    druid_cat_attack_t( "rake", player, SCHOOL_BLEED, TREE_FERAL )
  {
    druid_t* p = player -> cast_druid();

    option_t options[] =
    {
      { NULL, OPT_UNKNOWN, NULL }
    };
    parse_options( options, options_str );

    id = 1822;
    parse_data( p -> player_data );

    adds_combo_points = true;
    may_crit          = true;

    if ( p -> set_bonus.tier9_2pc_melee() ) num_ticks++;

  }
  virtual void tick()
  {
    druid_cat_attack_t::tick();
    trigger_t8_2pc_melee( this );
  }
};

// Rip ======================================================================

struct rip_t : public druid_cat_attack_t
{
  double base_dmg_per_point;
  double ap_per_point;

  rip_t( player_t* player, const std::string& options_str ) :
    druid_cat_attack_t( "rip", player, SCHOOL_BLEED, TREE_FERAL )
  {
    druid_t* p = player -> cast_druid();

    option_t options[] =
    {
      { NULL, OPT_UNKNOWN, NULL }
    };
    parse_options( options, options_str );

    id = 1079;
    parse_data( p -> player_data );
    base_td_init       = base_td;
    base_dmg_per_point = p->player_data.effect_bonus(p->player_data.spell_effect_id(id, 1), p->player_data.spell_scaling_class(id), p->level);
    ap_per_point       = 0.023;

    may_crit              = false;
    requires_combo_points = true;

    if ( p -> set_bonus.tier10_2pc_melee() )
      base_cost -= 10;

    num_ticks += ( p -> set_bonus.tier7_2pc_melee() * 2 );
    if ( p -> glyphs.rip )
      base_multiplier *= 1.15;

    if ( p -> set_bonus.tier9_4pc_melee() ) base_crit += 0.05;
  }

  virtual void execute()
  {
    druid_t* p = player -> cast_druid();
    druid_cat_attack_t::execute();
    if ( result_is_hit() )
    {
      added_ticks = 0;
      num_ticks = 8 + ( p -> set_bonus.tier7_2pc_melee() * 2 );
      update_ready();
    }
  }

  virtual void tick()
  {
    druid_cat_attack_t::tick();
    trigger_t8_2pc_melee( this );
  }

  virtual void player_buff()
  {
    druid_t* p = player -> cast_druid();
    base_td = base_td_init + p->buffs_combo_points->stack() * base_dmg_per_point;
    tick_power_mod = p->buffs_combo_points->stack() * ap_per_point;
    druid_cat_attack_t::player_buff();
  }
};

// Savage Roar =============================================================

struct savage_roar_t : public druid_cat_attack_t
{
  double buff_value;
  savage_roar_t( player_t* player, const std::string& options_str ) :
    druid_cat_attack_t( "savage_roar", player, SCHOOL_PHYSICAL, TREE_FERAL )
  {
    druid_t* p = player -> cast_druid();

    option_t options[] =
    {
      { NULL, OPT_UNKNOWN, NULL }
    };
    parse_options( options, options_str );
    requires_combo_points = true;
    base_cost = 25;
    id = 52610;
    harmful = false;

    buff_value = 0.50;
    if ( p -> glyphs.savage_roar ) buff_value += 0.03;
  }

  virtual void execute()
  {
    druid_t* p = player -> cast_druid();
    double duration = 9.0 + 5.0 * p -> buffs_combo_points -> stack();
    duration += 4.0 * p -> talents.endless_carnage -> rank();
    if ( p -> set_bonus.tier8_4pc_melee() ) duration += 8.0;

    // execute clears CP, so has to be after calculation duration
    druid_cat_attack_t::execute();

    p -> buffs_savage_roar -> buff_duration = duration;
    p -> buffs_savage_roar -> trigger( 1, buff_value );
    //p -> buffs_combo_points -> expire();
  }
};

// Shred ===================================================================

struct shred_t : public druid_cat_attack_t
{
  int omen_of_clarity;
  int extend_rip;

  shred_t( player_t* player, const std::string& options_str ) :
    druid_cat_attack_t( "shred", player, SCHOOL_PHYSICAL, TREE_FERAL ),
    omen_of_clarity( 0 ), extend_rip( 0 )
  {
    druid_t* p = player -> cast_druid();

    option_t options[] =
    {
      { "omen_of_clarity", OPT_BOOL, &omen_of_clarity },
      { "extend_rip",      OPT_BOOL, &extend_rip      },
      { NULL, OPT_UNKNOWN, NULL }
    };
    parse_options( options, options_str );

    id = 5221;
    parse_data( p -> player_data );

    weapon = &( p -> main_hand_weapon );
    requires_position  = POSITION_BACK;
    adds_combo_points  = true;
    may_crit           = true;
  }

  virtual void execute()
  {
    druid_t* p = player -> cast_druid();
    druid_cat_attack_t::execute();
    if ( p -> glyphs.shred &&
         p -> dots_rip -> ticking()  &&
         p -> dots_rip -> action -> added_ticks < 3 )
    {
      p -> dots_rip -> action -> extend_duration( 1 );
    }
    if ( result_is_hit() )
    {
      trigger_infected_wounds( this );
    }
  }

  virtual void player_buff()
  {
    druid_t*  p = player -> cast_druid();
    target_t* t = target;

    druid_cat_attack_t::player_buff();

    if ( t -> debuffs.mangle -> up() || t -> debuffs.blood_frenzy_bleed -> up() ) player_multiplier *= 1.30;

    if ( t -> debuffs.bleeding -> check() )
    {
      player_multiplier *= 1 + 0.01 * p -> talents.rend_and_tear -> mod_additive( P_CRIT );
    }

  }

  virtual bool ready()
  {
    druid_t* p = player -> cast_druid();

    if ( omen_of_clarity )
      if ( ! p -> buffs_omen_of_clarity -> may_react() )
        return false;

    if ( extend_rip )
      if ( ! p -> glyphs.shred ||
           ! p -> dots_rip -> ticking() ||
           ( p -> dots_rip -> action -> added_ticks == 3 ) )
        return false;

    return druid_cat_attack_t::ready();
  }
};

// Tigers Fury =============================================================

struct tigers_fury_t : public druid_cat_attack_t
{
  tigers_fury_t( player_t* player, const std::string& options_str ) :
      druid_cat_attack_t( "tigers_fury", player, SCHOOL_PHYSICAL, TREE_FERAL )
  {
    druid_t* p = player -> cast_druid();

    option_t options[] =
    {
      { NULL, OPT_UNKNOWN, NULL }
    };
    parse_options( options, options_str );

    id = 5217;
    parse_data( p -> player_data );

    if ( p -> set_bonus.tier7_4pc_melee() ) 
      cooldown -> duration -= 3.0;
    if ( p -> glyphs.tigers_fury ) 
      cooldown -> duration -= 3.0;

    trigger_gcd = 0;
  }

  virtual void execute()
  {
    if ( sim -> log ) log_t::output( sim, "%s performs %s", player -> name(), name() );

    druid_t* p = player -> cast_druid();

    if ( p -> talents.king_of_the_jungle -> rank() )
    {
      p -> resource_gain( RESOURCE_ENERGY, p -> talents.king_of_the_jungle -> effect_base_value( 2 ), p -> gains_tigers_fury );
    }

    p -> buffs_tigers_fury -> trigger( 1, 0.15 );
    update_ready();
  }

  virtual bool ready()
  {
    druid_t* p = player -> cast_druid();

    if ( p -> buffs_berserk -> check() )
      return false;

    return druid_cat_attack_t::ready();
  }
};

// Ferocious Bite ============================================================

struct ferocious_bite_t : public druid_cat_attack_t
{
  struct double_pair { double min, max; };
  double excess_energy;
  double_pair* combo_point_dmg;

  ferocious_bite_t( player_t* player, const std::string& options_str ) :
      druid_cat_attack_t( "ferocious_bite", player, SCHOOL_PHYSICAL, TREE_FERAL ),
      excess_energy( 0.0 )
  {
    druid_t* p = player -> cast_druid();

    option_t options[] =
    {
      { NULL, OPT_UNKNOWN, NULL }
    };
    parse_options( options, options_str );

    id = 48577;

    requires_combo_points = true;
    may_crit  = true;
    if ( p -> set_bonus.tier9_4pc_melee() ) base_crit += 0.05;

    base_cost = 35;

    base_multiplier *= 1.0 + ( p -> talents.feral_aggression -> rank() * 0.05 );

    static double_pair dmg_78[] = { { 410, 550 }, { 700, 840 }, { 990, 1130 }, { 1280, 1420 }, { 1570, 1710 } };
    static double_pair dmg_72[] = { { 334, 682 }, { 570, 682 }, { 806,  918 }, { 1042, 1154 }, { 1278, 1390 } };
    static double_pair dmg_63[] = { { 226, 292 }, { 395, 461 }, { 564,  630 }, {  733,  799 }, {  902,  968 } };
    static double_pair dmg_60[] = { { 199, 259 }, { 346, 406 }, { 493,  533 }, {  640,  700 }, {  787,  847 } };

    combo_point_dmg   = ( p -> level >= 78 ? dmg_78 :
                          p -> level >= 72 ? dmg_72 :
                          p -> level >= 63 ? dmg_63 :
                          dmg_60 );
    
  }

  virtual void execute()
  {
    druid_t* p = player -> cast_druid();

    base_dd_min = combo_point_dmg[ p -> buffs_combo_points -> current_stack - 1 ].min;
    base_dd_max = combo_point_dmg[ p -> buffs_combo_points -> current_stack - 1 ].max;

    direct_power_mod = 0.07 * p -> buffs_combo_points -> stack();
    // consumes up to 35 additional energy to increase damage by up to 100%.
    // Assume 100/35 = 2.857% per additional energy consumed
    
    // Glyph: Your Ferocious Bite ability no longer converts extra energy into additional damage.
    if ( p -> glyphs.ferocious_bite )
    {
      excess_energy     = 0;
    }
    else
    {
      excess_energy = ( p -> resource_current[ RESOURCE_ENERGY ] - druid_cat_attack_t::cost() );
  
      if ( excess_energy > 35 )
      {
        excess_energy     = 35;
      }
      else if ( excess_energy < 0 )
      {
        excess_energy     = 0;
      }
    }

    druid_cat_attack_t::execute();

    if ( result_is_hit() )
    {
      if ( p -> rng_blood_in_the_water -> roll( p -> talents.blood_in_the_water -> rank() * 0.5 ) )
      {
        p -> dots_rip -> action -> refresh_duration();
      }
    }
  }

  virtual void consume_resource()
  {
    // Ferocious Bite consumes 35+x energy, with 0 <= x <= 35.
    // Consumes the base_cost and handles Omen of Clarity
    druid_cat_attack_t::consume_resource();

    if ( result_is_hit() )
    {
      // Let the additional energy consumption create it's own debug log entries.
      if ( sim -> debug )
        log_t::output( sim, "%s consumes an additional %.1f %s for %s", player -> name(),
                       excess_energy, util_t::resource_type_string( resource ), name() );

      player -> resource_loss( resource, excess_energy );
      stats -> consume_resource( excess_energy );
    }

  }

  virtual void player_buff()
  {
    druid_t* p = player -> cast_druid();

    druid_cat_attack_t::player_buff();

    player_multiplier *= 1.0 + excess_energy / 35.0;

    if ( target -> debuffs.bleeding -> check() )
    {
      player_crit += 0.01 * p -> talents.rend_and_tear -> effect_base_value( 2 );
    }
  }
};

// ==========================================================================
// Druid Bear Attack
// ==========================================================================

// druid_bear_attack_t::parse_options ======================================

void druid_bear_attack_t::parse_options( option_t*          options,
                                         const std::string& options_str )
{
  option_t base_options[] =
  {
    { "berserk",         OPT_INT,  &berserk             },
    { "rage>",           OPT_FLT,  &min_rage            },
    { "rage<",           OPT_FLT,  &max_rage            },
    { "mangle>",         OPT_FLT,  &min_mangle_expire   },
    { "mangle<",         OPT_FLT,  &max_mangle_expire   },
    { "lacerate>",       OPT_FLT,  &min_lacerate_expire },
    { "lacerate<",       OPT_FLT,  &max_lacerate_expire },
    { "lacerate_stack>", OPT_INT,  &min_lacerate_stack  },
    { "lacerate_stack<", OPT_INT,  &max_lacerate_stack  },
    { NULL, OPT_UNKNOWN, NULL }
  };
  std::vector<option_t> merged_options;
  attack_t::parse_options( merge_options( merged_options, options, base_options ), options_str );
}

// druid_bear_attack_t::cost ===============================================

double druid_bear_attack_t::cost() SC_CONST
{
  druid_t* p = player -> cast_druid();
  double c = attack_t::cost();
  if ( harmful && p -> buffs_omen_of_clarity -> check() ) return 0;
  return c;
}

// druid_bear_attack_t::consume_resource ===================================

void druid_bear_attack_t::consume_resource()
{
  druid_t* p = player -> cast_druid();
  attack_t::consume_resource();
  if ( harmful && p -> buffs_omen_of_clarity -> up() )
  {
    // Treat the savings like a rage gain.
    double amount = attack_t::cost();
    if ( amount > 0 )
    {
      p -> gains_omen_of_clarity -> add( amount );
      p -> buffs_omen_of_clarity -> expire();
    }
  }
}

// druid_bear_attack_t::execute ============================================

void druid_bear_attack_t::execute()
{
  attack_t::execute();
  if ( result_is_hit() )
  {
    if ( result == RESULT_CRIT )
    {
      trigger_primal_fury( this );
    }
  }
}

// druid_bear_attack_t::player_buff ========================================

void druid_bear_attack_t::player_buff()
{
  druid_t* p = player -> cast_druid();
  attack_t::player_buff();

  if ( p -> talents.master_shapeshifter -> rank() )
  {
    player_multiplier *= 1.0 + p -> talents.master_shapeshifter -> rank() * 0.04;
  }
  if ( p -> talents.king_of_the_jungle -> rank() && p -> buffs_enrage -> up() )
  {
    player_multiplier *= 1.0 + p -> talents.king_of_the_jungle -> rank() * 0.05;
  }
}

// druid_bear_attack_t::ready ==============================================

bool druid_bear_attack_t::ready()
{
  if ( ! attack_t::ready() )
    return false;

  druid_t*  p = player -> cast_druid();
  target_t* t = target;

  if ( ! p -> buffs_bear_form -> check() )
    return false;

  if ( berserk && ! p -> buffs_berserk -> check() )
    return false;

  if ( min_rage > 0 )
    if ( p -> resource_current[ RESOURCE_RAGE ] < min_rage )
      return false;

  if ( max_rage > 0 )
    if ( p -> resource_current[ RESOURCE_RAGE ] > max_rage )
      return false;

  if ( min_mangle_expire > 0 )
    if ( t -> debuffs.mangle -> remains_lt( min_mangle_expire ) )
      return false;

  if ( max_mangle_expire > 0 )
    if ( t -> debuffs.mangle -> remains_gt( max_mangle_expire ) )
      return false;

  if ( min_lacerate_expire > 0 )
    if ( p -> buffs_lacerate -> remains_lt( min_lacerate_expire ) )
      return false;

  if ( max_lacerate_expire > 0 )
    if ( p -> buffs_lacerate -> remains_gt( max_lacerate_expire ) )
      return false;

  if ( min_lacerate_stack > 0 )
    if ( p -> buffs_lacerate -> current_stack < min_lacerate_stack )
      return false;

  if ( max_lacerate_stack > 0 )
    if ( p -> buffs_lacerate -> current_stack > max_lacerate_stack )
      return false;

  return true;
}

// Bear Melee Attack =======================================================

struct bear_melee_t : public druid_bear_attack_t
{
  bear_melee_t( player_t* player ) :
    druid_bear_attack_t( "bear_melee", player, SCHOOL_PHYSICAL, TREE_NONE, /*special*/false )
  {
    base_dd_min = base_dd_max = 1;
    background  = true;
    repeating   = true;
    trigger_gcd = 0;
    base_cost   = 0;
    may_crit    = true;
  }

  virtual double execute_time() SC_CONST
  {
    if ( ! player -> in_combat ) return 0.01;
    return druid_bear_attack_t::execute_time();
  }

  virtual void execute()
  {
    druid_bear_attack_t::execute();
    if ( result_is_hit() )
    {
      trigger_fury_swipes( this );
      trigger_rage_gain( this );
      trigger_omen_of_clarity( this );
    }
  }
};

// Bash ====================================================================

struct bash_t : public druid_bear_attack_t
{
  bash_t( player_t* player, const std::string& options_str ) :
    druid_bear_attack_t( "bash", player, SCHOOL_PHYSICAL, TREE_FERAL )
  {
    druid_t* p = player -> cast_druid();

    option_t options[] =
    {
      { NULL, OPT_UNKNOWN, NULL }
    };
    parse_options( options, options_str );

    base_cost = 10;
    cooldown -> duration  = 60 - p -> talents.brutal_impact -> rank() * 15;
  }

  virtual bool ready()
  {
    if ( ! target -> debuffs.casting -> check() ) return false;
    return druid_bear_attack_t::ready();
  }
};

// Berserk =================================================================

struct berserk_bear_t : public druid_bear_attack_t
{
  berserk_bear_t( player_t* player, const std::string& options_str ) :
    druid_bear_attack_t( "berserk_bear", player )
  {
    druid_t* p = player -> cast_druid();
    check_talent( p -> talents.berserk -> rank() );

    option_t options[] =
    {
      { NULL, OPT_UNKNOWN, NULL }
    };

    parse_options( options, options_str );

    cooldown = p -> get_cooldown( "berserk" );
    cooldown -> duration = 180;
    id = 50334;
  }

  virtual void execute()
  {
    druid_t* p = player -> cast_druid();
    if ( sim -> log ) log_t::output( sim, "%s performs %s", p -> name(), name() );
    p -> buffs_berserk -> trigger();
    p -> cooldowns_mangle_bear -> reset();
    update_ready();
  }
};

// Lacerate ================================================================

struct lacerate_t : public druid_bear_attack_t
{
  lacerate_t( player_t* player, const std::string& options_str ) :
      druid_bear_attack_t( "lacerate",  player, SCHOOL_BLEED, TREE_FERAL )
  {
    druid_t* p = player -> cast_druid();

    option_t options[] =
    {
      { NULL,     OPT_UNKNOWN, NULL       }
    };
    parse_options( options, options_str );

    static rank_t ranks[] =
    {
      { 80, 3, 88, 88, 64, 15 },
      { 73, 2, 70, 70, 51, 15 },
      { 66, 1, 31, 31, 31, 15 },
      { 0, 0, 0, 0, 0, 0 }
    };
    init_rank( ranks );

    may_crit = true;
    num_ticks = 5;
    base_tick_time = 3.0;
    direct_power_mod = 0.01;
    tick_power_mod   = 0.01;
    tick_may_crit    = true;
    dot_behavior = DOT_REFRESH;

    if ( p -> set_bonus.tier9_2pc_melee() ) base_td_multiplier *= 1.05;
  }

  virtual void execute()
  {
    druid_t* p = player -> cast_druid();
    druid_bear_attack_t::execute();
    if ( result_is_hit() )
    {
      p -> buffs_lacerate -> trigger();
      base_td_multiplier = p -> buffs_lacerate -> current_stack;
    }
  }

  virtual void tick()
  {
    druid_bear_attack_t::tick();
    trigger_t8_2pc_melee( this );
  }

  virtual void last_tick()
  {
    druid_t* p = player -> cast_druid();
    druid_bear_attack_t::last_tick();
    p -> buffs_lacerate -> expire();
  }
};

// Mangle (Bear) ===========================================================

struct mangle_bear_t : public druid_bear_attack_t
{
  mangle_bear_t( player_t* player, const std::string& options_str ) :
      druid_bear_attack_t( "mangle_bear", player, SCHOOL_PHYSICAL, TREE_FERAL )
  {
    druid_t* p = player -> cast_druid();

    option_t options[] =
    {
      { NULL, OPT_UNKNOWN, NULL }
    };
    parse_options( options, options_str );

    static rank_t ranks[] =
    {
      { 80, 5, 260, 260, 0, 20 },
      { 75, 4, 219, 219, 0, 20 },
      { 68, 3, 135, 135, 0, 20 },
      { 58, 2, 105, 105, 0, 20 },
      {  0, 0,   0,   0, 0,  0 }
    };
    init_rank( ranks, 48564 );

    weapon = &( p -> main_hand_weapon );
    weapon_multiplier *= 1.15;

    may_crit = true;

    base_multiplier *= 1.0 + p -> glyphs.mangle * 0.10;

    cooldown = p -> get_cooldown( "mangle_bear" );
    cooldown -> duration = 6.0;
  }

  virtual void execute()
  {
    druid_t* p = player -> cast_druid();
    druid_bear_attack_t::execute();
    if ( p -> buffs_berserk -> up() ) cooldown -> reset();
    if ( result_is_hit() )
    {
      target -> debuffs.mangle -> trigger();
      trigger_infected_wounds( this );
    }
  }
};

// Maul ====================================================================

struct maul_t : public druid_bear_attack_t
{
  maul_t( player_t* player, const std::string& options_str ) :
      druid_bear_attack_t( "maul",  player, SCHOOL_PHYSICAL, TREE_FERAL )
  {
    druid_t* p = player -> cast_druid();

    option_t options[] =
    {
      { NULL, OPT_UNKNOWN, NULL }
    };
    parse_options( options, options_str );

    static rank_t ranks[] =
    {
      { 79, 10, 578, 578, 0, 15 },
      { 73,  9, 472, 472, 0, 15 },
      { 67,  8, 290, 290, 0, 15 },
      { 58,  7, 192, 192, 0, 15 },
      { 0, 0, 0, 0, 0, 0 }
    };
    init_rank( ranks );

    trigger_gcd  = 0;
    may_crit     = true;

    // TODO: scaling?
    weapon = &( p -> main_hand_weapon );
    //normalize_weapon_speed = false;

    cooldown -> duration = 3;
  }

  virtual void execute()
  {
    druid_bear_attack_t::execute();
    if ( result_is_hit() )
    {
      trigger_omen_of_clarity( this );
      trigger_infected_wounds( this );
    }
  }

  virtual void player_buff()
  {
    druid_t*  p = player -> cast_druid();
    target_t* t = target;

    druid_bear_attack_t::player_buff();

    if ( t -> debuffs.mangle -> up() || t -> debuffs.blood_frenzy_bleed -> up() ) player_multiplier *= 1.30;

    if ( t -> debuffs.bleeding -> check() )
    {
      player_multiplier *= 1 + 0.20/3.0 * p -> talents.rend_and_tear -> rank();
    }

  }

};

// Swipe (Bear) ============================================================

struct swipe_bear_t : public druid_bear_attack_t
{
  swipe_bear_t( player_t* player, const std::string& options_str ) :
      druid_bear_attack_t( "swipe_bear", player, SCHOOL_PHYSICAL, TREE_FERAL )
  {
    // druid_t* p = player -> cast_druid();

    option_t options[] =
    {
      { NULL, OPT_UNKNOWN, NULL }
    };
    parse_options( options, options_str );

    static rank_t ranks[] =
    {
      { 77, 8, 108, 108, 0, 20 },
      { 72, 7,  95,  95, 0, 20 },
      { 64, 6,  76,  76, 0, 20 },
      { 54, 5,  54,  54, 0, 20 },
      { 44, 4,  37,  37, 0, 20 },
      { 34, 3,  19,  19, 0, 20 },
      { 24, 2,  13,  13, 0, 20 },
      { 16, 1,   9,   9, 0, 20 },
      {  0, 0,   0,   0, 0,  0 }
    };
    init_rank( ranks, 48562 );

    weapon_multiplier = 0;
    direct_power_mod = 0.07;
    may_crit = true;
  }

  virtual void assess_damage( double amount,
                               int    dmg_type )
  {
    druid_bear_attack_t::assess_damage( amount, dmg_type );

    for ( int i=0; i < target -> adds_nearby && i < 10; i ++ )
    {
      druid_bear_attack_t::additional_damage( amount, dmg_type );
    }
  }
};

// ==========================================================================
// Druid Spell
// ==========================================================================

// druid_spell_t::parse_options ============================================

void druid_spell_t::parse_options( option_t*          options,
                                   const std::string& options_str )
{
  option_t base_options[] =
  {
    { "skip_on_eclipse",  OPT_INT, &skip_on_eclipse        },
    { NULL, OPT_UNKNOWN, NULL }
  };
  std::vector<option_t> merged_options;
  spell_t::parse_options( merge_options( merged_options, options, base_options ), options_str );
}

// druid_spell_t::ready ====================================================

bool druid_spell_t::ready()
{
  if ( ! spell_t::ready() )
    return false;

  druid_t*  p = player -> cast_druid();

  if ( skip_on_eclipse > 0 )
    if ( p -> buffs_eclipse_lunar -> may_react() || p -> buffs_eclipse_solar -> may_react() )
      return false;

  return true;
}

// druid_spell_t::cost_reduction ===========================================

double druid_spell_t::cost_reduction() SC_CONST
{
  druid_t* p = player -> cast_druid();
  double   cr = 0.0;

  cr += p -> talents.moonglow -> base_value();
  return cr;
}

// druid_spell_t::cost =====================================================

double druid_spell_t::cost() SC_CONST
{
  druid_t* p = player -> cast_druid();
  if ( harmful && p -> buffs_omen_of_clarity -> check() ) return 0;
  double c = spell_t::cost();
  c *= 1.0 + cost_reduction();
  if ( c < 0 ) c = 0.0;  
  return c;
}

// druid_spell_t::haste ====================================================

double druid_spell_t::haste() SC_CONST
{
  druid_t* p = player -> cast_druid();
  double h =  spell_t::haste();

  h *= 1.0 / ( 1.0 +  p -> buffs_natures_grace -> value() );

  return h;
}

// druid_spell_t::execute_time =============================================

double druid_spell_t::execute_time() SC_CONST
{
  druid_t* p = player -> cast_druid();
  if ( p -> buffs_natures_swiftness -> check() ) return 0;
  return spell_t::execute_time();
}

// druid_spell_t::schedule_execute =========================================

void druid_spell_t::schedule_execute()
{
  druid_t* p = player -> cast_druid();

  spell_t::schedule_execute();

  if ( base_execute_time > 0 )
  {
    p -> buffs_natures_swiftness -> expire();
  }
}

// druid_spell_t::execute ==================================================

void druid_spell_t::execute()
{
  druid_t* p = player -> cast_druid();

  spell_t::execute();

  if ( result == RESULT_CRIT ) p -> buffs_t11_4pc_caster -> decrement();

  if ( ! aoe )
  {
    trigger_omen_of_clarity( this );
  }
}

// druid_spell_t::consume_resource =========================================

void druid_spell_t::consume_resource()
{
  druid_t* p = player -> cast_druid();
  spell_t::consume_resource();
  if ( harmful && p -> buffs_omen_of_clarity -> up() )
  {
    // Treat the savings like a mana gain.
    double amount = spell_t::cost();
    if ( amount > 0 )
    {
      p -> gains_omen_of_clarity -> add( amount );
      p -> buffs_omen_of_clarity -> expire();
    }
  }
}

// druid_spell_t::player_buff ==============================================

void druid_spell_t::player_buff()
{
  druid_t* p = player -> cast_druid();

  spell_t::player_buff();

  if ( p -> buffs_moonkin_form -> check() )
  {
    player_multiplier *= 1.0 + p -> talents.master_shapeshifter -> rank() * 0.04;
  }
  if ( school == SCHOOL_ARCANE || school == SCHOOL_NATURE || school == SCHOOL_SPELLSTORM )
  {
    player_multiplier *= 1.0 + 0.01 * p -> talents.balance_of_power -> effect_base_value( 1 );

    player_multiplier *= 1.0 + p -> buffs_t10_2pc_caster -> value();
    // Moonfury: Arcane and Nature spell damage increased by 25%
    // One of the bonuses for choosing balance spec
    if ( p -> primary_tree() == TREE_BALANCE )
      player_multiplier *= 1.0 + 0.01 * p -> spec_moonfury -> mod_additive( P_GENERIC );

    if ( p -> buffs_moonkin_form -> check() )
      player_multiplier *= 1.10;
  }
  // Both eclipse buffs need their own checks
  // Eclipse increases wrath damage by 1.5% per mastery point
  if ( school == SCHOOL_ARCANE || school == SCHOOL_SPELLSTORM )
    if ( p -> buffs_eclipse_lunar -> up() )
      player_multiplier *= 1.25 + p -> composite_mastery() * 0.0001 * p -> mastery_total_eclipse -> effect_base_value( 2 );

  if ( school == SCHOOL_NATURE || school == SCHOOL_SPELLSTORM )
    if ( p -> buffs_eclipse_solar -> up() )
      player_multiplier *= 1.25 + p -> composite_mastery() * 0.0001 * p -> mastery_total_eclipse -> effect_base_value( 2 );

  player_multiplier *= 1.0 + p -> talents.earth_and_moon -> rank() * 0.02;
  
  player_crit += 0.33 * p -> buffs_t11_4pc_caster -> stack();

}

// druid_spell_t::target_debuff ============================================

void druid_spell_t::target_debuff( int dmg_type )
{
  //druid_t*  p = player -> cast_druid();
  //target_t* t = target;
  spell_t::target_debuff( dmg_type );
}

// Auto Attack =============================================================

struct auto_attack_t : public action_t
{
  auto_attack_t( player_t* player, const std::string& options_str ) :
    action_t( ACTION_OTHER, "auto_attack", player )
  {
    trigger_gcd = 0;
  }

  virtual void execute()
  {
    druid_t* p = player -> cast_druid();
    p -> main_hand_attack -> weapon = &( p -> main_hand_weapon );
    p -> main_hand_attack -> base_execute_time = p -> main_hand_weapon.swing_time;
    p -> main_hand_attack -> schedule_execute();
  }

  virtual bool ready()
  {
    druid_t* p = player -> cast_druid();
    if ( p -> buffs.moving -> check() ) return false;
    return( p -> main_hand_attack -> execute_event == 0 ); // not swinging
  }
};

// Enrage ==================================================================

struct enrage_t : public druid_spell_t
{
  double max_rage;

  enrage_t( player_t* player, const std::string& options_str ) :
    druid_spell_t( "enrage", player, SCHOOL_PHYSICAL, TREE_FERAL )
  {
    option_t options[] =
    {
      { "rage<", OPT_FLT,     &max_rage },
      { NULL,    OPT_UNKNOWN, NULL      }
    };
    max_rage = 0;
    parse_options( options, options_str );
    trigger_gcd = 0;
    cooldown -> duration = 60;
  }

  virtual void execute()
  {
    druid_t* p = player -> cast_druid();
    if ( sim -> log ) log_t::output( sim, "%s performs %s", p -> name(), name() );
    p -> buffs_enrage -> trigger();
    p -> resource_gain( RESOURCE_RAGE, 20, p -> gains_enrage );
    update_ready();
  }

  virtual bool ready()
  {
    druid_t* p = player -> cast_druid();
    if ( ! p -> buffs_bear_form -> check() )
      return false;
    if ( max_rage > 0 )
      if ( p -> resource_current[ RESOURCE_RAGE ] > max_rage )
        return false;
    return druid_spell_t::ready();
  }
};

// Faerie Fire (Feral) ======================================================

struct faerie_fire_feral_t : public druid_spell_t
{
  int debuff_only;

  faerie_fire_feral_t( player_t* player, const std::string& options_str ) :
    druid_spell_t( "faerie_fire_feral", player, SCHOOL_PHYSICAL, TREE_FERAL ), debuff_only( 0 )
  {
    option_t options[] =
    {
      { "debuff_only", OPT_BOOL, &debuff_only },
      { NULL, OPT_UNKNOWN, NULL }
    };
    parse_options( options, options_str );

    may_crit = true;
    cooldown -> duration = 6.0;
    base_dd_min = base_dd_max = 1;
    direct_power_mod = 1.0;
    base_spell_power_multiplier = 0;
    base_attack_power_multiplier = 0.15;
    id = 16857;
  }

  virtual void execute()
  {
    druid_t* p = player -> cast_druid();
    if ( sim -> log ) log_t::output( sim, "%s performs %s", p -> name(), name() );
    if ( p -> buffs_bear_form -> check() )
    {
      // The damage component is only active in (Dire) Bear Form
      spell_t::execute();
    }
    else
    {
      update_ready();
    }
    target -> debuffs.faerie_fire -> trigger( 1 + p -> talents.feral_aggression -> rank(), 0.04 );
  }

  virtual bool ready()
  {
    if ( debuff_only )
      if ( target -> debuffs.faerie_fire -> up() )
        return false;

    return druid_spell_t::ready();
  }
};

// Faerie Fire Spell =======================================================

struct faerie_fire_t : public druid_spell_t
{
  faerie_fire_t( player_t* player, const std::string& options_str ) :
      druid_spell_t( "faerie_fire", player, SCHOOL_NATURE, TREE_BALANCE )
  {
    // TODO: convert to cata mechanic
    druid_t* p = player -> cast_druid();

    option_t options[] =
    {
      { NULL, OPT_UNKNOWN, NULL }
    };
    parse_options( options, options_str );

    base_cost = p -> resource_base[ RESOURCE_MANA ] * 0.08;
    id = 770;
  }

  virtual void execute()
  {
    druid_t*  p = player -> cast_druid();
    if ( sim -> log ) log_t::output( sim, "%s performs %s", p -> name(), name() );
    consume_resource();
    target -> debuffs.faerie_fire -> trigger( 1, 0.04 );
    target -> debuffs.faerie_fire -> source = p;
  }

  virtual bool ready()
  {
    // druid_t*  p = player -> cast_druid();
    // target_t* t = target;

    return druid_spell_t::ready();
  }
};

// Innervate Spell =========================================================

struct innervate_t : public druid_spell_t
{
  int trigger;
  player_t* innervate_target;

  innervate_t( player_t* player, const std::string& options_str ) :
      druid_spell_t( "innervate", player, SCHOOL_NATURE, TREE_BALANCE ), trigger( 0 )
  {
    druid_t* p = player -> cast_druid();

    std::string target_str;
    option_t options[] =
    {
      { "trigger", OPT_INT,    &trigger    },
      { "target",  OPT_STRING, &target_str },
      { NULL, OPT_UNKNOWN, NULL }
    };
    parse_options( options, options_str );

    id = 29166;

    base_cost = 0.0;
    base_execute_time = 0;
    cooldown -> duration  = 180;

    harmful   = false;

    // If no target is set, assume we have innervate for ourself
    innervate_target = target_str.empty() ? p : sim -> find_player( target_str );
    assert ( innervate_target != 0 );
  }
  virtual void execute()
  {
    druid_t* p = player -> cast_druid();
    if ( sim -> log ) log_t::output( sim, "%s performs %s", player -> name(),name() );

    consume_resource();
    update_ready();

    // 20% over 10s base
    double gain = 0.20;

    if ( innervate_target == player )
    {
      gain += 0.01 * p -> talents.dreamstate -> effect_base_value( 1 );
    }
    else
    {
      // Either Dreamstate increases innervate OR you get glyph of innervate
      p -> buffs_glyph_of_innervate -> trigger( 1, p -> resource_max[ RESOURCE_MANA ] * gain / 20.0 );
    }
    innervate_target -> buffs.innervate -> trigger( 1, p -> resource_max[ RESOURCE_MANA ] * gain / 10.0);
  }

  virtual bool ready()
  {
    if ( ! druid_spell_t::ready() )
      return false;

    if ( trigger < 0 )
      return ( player -> resource_current[ RESOURCE_MANA ] + trigger ) < 0;

    return ( player -> resource_max    [ RESOURCE_MANA ] -
             player -> resource_current[ RESOURCE_MANA ] ) > trigger;
  }
};

// Insect Swarm Spell ======================================================

struct insect_swarm_t : public druid_spell_t
{
  cooldown_t* starsurge_cd;
  insect_swarm_t( player_t* player, const std::string& options_str ) :
      druid_spell_t( "insect_swarm", player, SCHOOL_NATURE, TREE_BALANCE ),
      starsurge_cd( 0 )
  {
    druid_t* p = player -> cast_druid();

    option_t options[] =
    {
      { NULL, OPT_UNKNOWN, NULL }
    };
    parse_options( options, options_str );
    
    id = 5570;
    parse_data( p -> player_data );
    
    base_execute_time = 0;
    base_tick_time    = 2.0;
    num_ticks         = 6;
    // Genesis, additional time is given in ms. Current structure requires it to be converted into ticks
    num_ticks        += (int) ( p -> talents.genesis -> effect_base_value( 3 ) / 2000.0 ); 
    tick_power_mod    = 0.2;
    tick_may_crit     = true;
    dot_behavior      = DOT_REFRESH;

    base_multiplier *= 1.0 + ( p -> glyphs.insect_swarm          ? 0.30 : 0.00 ) +
                             ( p -> set_bonus.tier7_2pc_caster() ? 0.10 : 0.00 );

    if ( p -> set_bonus.tier11_2pc_caster() )
      base_crit += 0.05;
    
    starsurge_cd = p -> get_cooldown( "starsurge" );
  }

  virtual void tick()
  {
    druid_spell_t::tick();
    druid_t* p = player -> cast_druid();
    p -> buffs_t8_4pc_caster -> trigger();
    if ( p -> buffs_shooting_stars -> trigger() )
      starsurge_cd -> reset();
  }

  virtual bool ready()
  {
    if ( ! druid_spell_t::ready() )
      return false;

    druid_t* p = player -> cast_druid();

    if ( skip_on_eclipse < 0 )
      if ( p -> buffs_eclipse_lunar -> check() )
        return false;

    return true;
  }
  
  virtual void execute()
  {
    druid_spell_t::execute();
    druid_t* p = player -> cast_druid();
    if ( result_is_hit() )
    {
      p -> buffs_natures_grace -> trigger( 1, p -> talents.natures_grace -> rank() * 0.05 );
    }
  }
};

// Moonfire Spell ===========================================================

struct moonfire_t : public druid_spell_t
{
  cooldown_t* starsurge_cd;
  int         start_num_ticks;
  moonfire_t( player_t* player, const std::string& options_str ) :
      druid_spell_t( "moonfire", player, SCHOOL_ARCANE, TREE_BALANCE ), 
      starsurge_cd( 0 ), start_num_ticks( 0 )
  {
    druid_t* p = player -> cast_druid();

    option_t options[] =
    {
      { NULL, OPT_UNKNOWN, NULL }
    };
    parse_options( options, options_str );

    id = 8921;
    parse_data( p -> player_data );

    base_execute_time = 0;
    base_tick_time    = 2.0;
    start_num_ticks   = 6;
    // Genesis, additional time is given in ms. Current structure requires it to be converted into ticks
    start_num_ticks  += (int) ( p -> talents.genesis -> effect_base_value( 3 ) / 2000.0 );
    num_ticks         = start_num_ticks;
    may_crit          = true;
    tick_may_crit     = true;
    dot_behavior      = DOT_REFRESH;
    
    if ( p -> primary_tree() == TREE_BALANCE )
      base_crit_bonus_multiplier *= 1.0 + p -> spec_moonfury -> mod_additive( P_CRIT_DAMAGE );

    base_dd_multiplier *= 1.0 + p -> talents.blessing_of_the_grove -> effect_base_value( 2 ) / 100.0;
    base_td_multiplier *= 1.0 + p -> glyphs.moonfire * 0.20;

    if ( p -> set_bonus.tier11_2pc_caster() )
      base_crit += 0.05;

    starsurge_cd = p -> get_cooldown( "starsurge" );
  }

  virtual void tick()
  {
    druid_spell_t::tick();
    druid_t* p = player -> cast_druid();
    if ( p -> buffs_shooting_stars -> trigger() )
      starsurge_cd -> reset();
  }
  virtual void player_buff()
  {
    druid_spell_t::player_buff();
    druid_t* p = player -> cast_druid();
    // +2/4/8% damage bonus only applies to direct damage
    player_multiplier *= 1.0 + util_t::talent_rank( p -> talents.lunar_shower -> rank(), 3, 0.15 ) * p -> buffs_lunar_shower -> stack();
  }
  virtual void execute()
  {
    druid_t* p = player -> cast_druid();
    
    // Sunfire, basically just makes MF benefit from lunar AND solar!
    if ( p -> talents.sunfire -> rank() )
      school = ( p -> buffs_eclipse_solar -> check() ? SCHOOL_NATURE : SCHOOL_ARCANE );
    
    druid_spell_t::execute();
    // Damage bonus only applies to direct damage
    // Get rid of it for the ticks, hacky :<
    player_multiplier /= 1.0 + util_t::talent_rank( p -> talents.lunar_shower -> rank(), 3, 0.15 ) * p -> buffs_lunar_shower -> stack();


    if ( result_is_hit() )
    {
      num_ticks   = start_num_ticks;
      added_ticks = 0;
      if ( p -> set_bonus.tier6_2pc_caster() ) num_ticks++;
      update_ready();

      // If moving trigger all 3 stacks, because it will stack up immediately
      p -> buffs_lunar_shower -> trigger( p -> buffs.moving -> check() ? 3 : 1 );
      p -> buffs_natures_grace -> trigger( 1, p -> talents.natures_grace -> rank() * 0.05 );
    }
  }
  virtual double cost_reduction() SC_CONST
  {
    // Costreduction from moonglow and lunar shower is additive
    druid_t* p = player -> cast_druid();
    double cr = druid_spell_t::cost_reduction();
    cr += (-0.10 * p -> buffs_lunar_shower -> stack() * p -> talents.lunar_shower -> rank() );
    return cr;
    
  }
  virtual bool ready()
  {
    if ( ! druid_spell_t::ready() )
      return false;

    druid_t* p = player -> cast_druid();

    if ( skip_on_eclipse < 0 )
      if ( p -> buffs_eclipse_solar -> check() )
        return false;

    return true;
  }
};

// Bear Form Spell ========================================================

struct bear_form_t : public druid_spell_t
{
  bear_form_t( player_t* player, const std::string& options_str ) :
      druid_spell_t( "bear_form", player, SCHOOL_NATURE, TREE_FERAL )
  {
    druid_t* p = player -> cast_druid();
    trigger_gcd = 0;
    base_execute_time = 0;
    base_cost = 0;
    id = 768;
    if ( ! p -> bear_melee_attack ) p -> bear_melee_attack = new bear_melee_t( p );
  }

  virtual void execute()
  {
    druid_t* p = player -> cast_druid();

    if ( sim -> log ) log_t::output( sim, "%s performs %s", p -> name(), name() );

    weapon_t* w = &( p -> main_hand_weapon );

    if ( w -> type != WEAPON_BEAST )
    {
      w -> type = WEAPON_BEAST;
      w -> school = SCHOOL_PHYSICAL;
      w -> damage = 54.8 * 2.5;
      w -> swing_time = 2.5;
    }

    // Force melee swing to restart if necessary
    if ( p -> main_hand_attack ) p -> main_hand_attack -> cancel();

    p -> main_hand_attack = p -> bear_melee_attack;
    p -> main_hand_attack -> weapon = w;

    if ( p -> buffs_cat_form     -> check() ) p -> buffs_cat_form     -> expire();
    if ( p -> buffs_moonkin_form -> check() ) p -> buffs_moonkin_form -> expire();

    p -> buffs_bear_form -> start();
    p -> base_gcd = 1.0;
    p -> reset_gcd();

    p -> dodge += 0.02 * p -> talents.feral_swiftness -> rank() + 0.03 * p -> talents.natural_reaction -> rank();
    p -> armor_multiplier += 3.7 * ( 1.0 + 0.11 * p -> talents.thick_hide -> rank() );

    if ( p -> talents.leader_of_the_pack -> rank() )
    {
      sim -> auras.leader_of_the_pack -> trigger();
    }
  }

  virtual bool ready()
  {
    druid_t* d = player -> cast_druid();
    return ! d -> buffs_bear_form -> check();
  }
};

// Cat Form Spell =========================================================

struct cat_form_t : public druid_spell_t
{
  cat_form_t( player_t* player, const std::string& options_str ) :
      druid_spell_t( "cat_form", player, SCHOOL_NATURE, TREE_FERAL )
  {
    druid_t* p = player -> cast_druid();
    trigger_gcd = 0;
    base_execute_time = 0;
    base_cost = 0;
    id = 768;
    if ( ! p -> cat_melee_attack ) p -> cat_melee_attack = new cat_melee_t( p );
  }

  virtual void execute()
  {
    druid_t* p = player -> cast_druid();

    if ( sim -> log ) log_t::output( sim, "%s performs %s", p -> name(), name() );

    weapon_t* w = &( p -> main_hand_weapon );

    if ( w -> type != WEAPON_BEAST )
    {
      // FIXME: If we really want to model switching between forms, the old values need to be saved somewhere
      w -> type = WEAPON_BEAST;
      w -> school = SCHOOL_PHYSICAL;
      w -> min_dmg /= w -> swing_time;
      w -> max_dmg /= w -> swing_time;
      w -> damage = ( w -> min_dmg + w -> max_dmg ) / 2;
      w -> swing_time = 1.0;
    }

    // Force melee swing to restart if necessary
    if ( p -> main_hand_attack ) p -> main_hand_attack -> cancel();

    p -> main_hand_attack = p -> cat_melee_attack;
    p -> main_hand_attack -> weapon = w;

    if ( p -> buffs_bear_form    -> check() ) p -> buffs_bear_form    -> expire();
    if ( p -> buffs_moonkin_form -> check() ) p -> buffs_moonkin_form -> expire();

    p -> dodge += 0.02 * p -> talents.feral_swiftness -> rank();

    p -> buffs_cat_form -> start();
    p -> base_gcd = 1.0;
    p -> reset_gcd();

    if ( p -> talents.leader_of_the_pack -> rank() )
    {
      sim -> auras.leader_of_the_pack -> trigger();
    }
  }

  virtual bool ready()
  {
    druid_t* d = player -> cast_druid();
    return ! d -> buffs_cat_form -> check();
  }
};

// Moonkin Form Spell =====================================================

struct moonkin_form_t : public druid_spell_t
{
  moonkin_form_t( player_t* player, const std::string& options_str ) :
      druid_spell_t( "moonkin_form", player, SCHOOL_NATURE, TREE_BALANCE )
  {
    druid_t* p = player -> cast_druid();
    check_talent( p -> talents.moonkin_form -> rank() );
    trigger_gcd = 0;
    base_execute_time = 0;
    base_cost = 0;
    id = 24858;
  }

  virtual void execute()
  {
    druid_t* p = player -> cast_druid();

    if ( sim -> log ) log_t::output( sim, "%s performs %s", p -> name(), name() );

    if ( p -> buffs_bear_form -> check() ) p -> buffs_bear_form -> expire();
    if ( p -> buffs_cat_form  -> check() ) p -> buffs_cat_form  -> expire();

    p -> buffs_moonkin_form -> start();

    sim -> auras.moonkin -> trigger();

    if ( p -> talents.furor -> rank() )
    {
      p -> attribute_multiplier[ ATTR_INTELLECT ] *= 1.0 + p -> talents.furor -> rank() * 0.02;
    }

    p -> armor_multiplier += 2.2;
  }

  virtual bool ready()
  {
    druid_t* d = player -> cast_druid();
    return ! d -> buffs_moonkin_form -> check();
  }
};

// Natures Swiftness Spell ==================================================

struct druids_swiftness_t : public druid_spell_t
{
  cooldown_t* sub_cooldown;
  dot_t*      sub_dot;

  druids_swiftness_t( player_t* player, const std::string& options_str ) :
    druid_spell_t( "natures_swiftness", player, SCHOOL_NATURE, TREE_RESTORATION ),
    sub_cooldown(0), sub_dot(0)
  {
    druid_t* p = player -> cast_druid();
    check_talent( p -> talents.natures_swiftness -> rank() );
    trigger_gcd = 0;
    cooldown -> duration = 180.0;
    if ( ! options_str.empty() )
    {
      // This will prevent Natures Swiftness from being called before the desired "fast spell" is ready to be cast.
      sub_cooldown = p -> get_cooldown( options_str );
      sub_dot      = p -> get_dot     ( options_str );
    }
    id = 17116;
  }

  virtual void execute()
  {
    if ( sim -> log ) log_t::output( sim, "%s performs %s", player -> name(), name() );
    druid_t* p = player -> cast_druid();
    p -> buffs_natures_swiftness -> trigger();
    update_ready();
  }

  virtual bool ready()
  {
    if ( sub_cooldown )
      if ( sub_cooldown -> remains() > 0 )
        return false;

    if ( sub_dot )
      if ( sub_dot -> remains() > 0 )
        return false;

    return druid_spell_t::ready();
  }
};

// Starfire Spell ===========================================================

struct starfire_t : public druid_spell_t
{
  std::string prev_str;
  int extend_moonfire;
  int instant;

  starfire_t( player_t* player, const std::string& options_str ) :
      druid_spell_t( "starfire", player, SCHOOL_ARCANE, TREE_BALANCE ),
      extend_moonfire( 0 ), instant( 0 )
  {
    druid_t* p = player -> cast_druid();

    option_t options[] =
    {
      { "extendmf", OPT_BOOL,   &extend_moonfire },
      { "prev",     OPT_STRING, &prev_str        },
      { "instant",  OPT_BOOL,   &instant         },
      { NULL, OPT_UNKNOWN, NULL }
    };
    parse_options( options, options_str );

    // Starfire is leanred at level 78, but gains 5 damage per level
    // so the actual damage range at 80 is: 1038 to 1222
    static rank_t ranks[] =
    {
      { 80, 11, 1038, 1222, 0, 0.16 },
      { 78, 10, 1028, 1212, 0, 0.16 },
      { 72,  9,  854, 1006, 0, 0.16 },
      { 67,  8,  818,  964, 0, 370  },
      { 60,  7,  693,  817, 0, 340  },
      { 0, 0, 0, 0, 0, 0 }
    };
    init_rank( ranks, 48465 );

    base_execute_time = 3.5;
    direct_power_mod  = ( base_execute_time / 3.5 );
    may_crit          = true;

    base_execute_time -= util_t::talent_rank( p -> talents.starlight_wrath -> rank(), 3, 0.15, 0.25, 0.5 );
    if ( p -> primary_tree() == TREE_BALANCE )
      base_crit_bonus_multiplier *= 1.0 + p -> spec_moonfury -> mod_additive( P_CRIT_DAMAGE );


    if ( p -> set_bonus.tier6_4pc_caster() ) base_crit += 0.05;
    if ( p -> set_bonus.tier7_4pc_caster() ) base_crit += 0.05;
    if ( p -> set_bonus.tier9_4pc_caster() ) base_multiplier *= 1.04;
  }

  virtual void schedule_execute()
  {
    druid_spell_t::schedule_execute();
    druid_t* p = player -> cast_druid();
    p -> buffs_t8_4pc_caster -> expire();
  }

  virtual void execute()
  {
    druid_t* p = player -> cast_druid();
    druid_spell_t::execute();

    if ( result_is_hit() )
    {
      trigger_earth_and_moon( this );

      if ( result == RESULT_CRIT )
      {
        trigger_t10_4pc_caster( player, direct_dmg, school );
      }
      if ( p -> glyphs.starfire )
      {
        if ( p -> dots_moonfire -> ticking() )
          if ( p -> dots_moonfire -> action -> added_ticks < 3 )
            p -> dots_moonfire -> action -> extend_duration( 1 );
      }
      // If Solar is up SF won't give you eclipse energy
      // This is probably to force druid to oscillate
      if ( ! p -> buffs_eclipse_solar -> check() )
      {
        // effect#2 of starfire spell => 92680
        // 92680 -> base_value == 20

        double gain = 20;
        if ( ! p -> buffs_eclipse_lunar -> check() 
          && p -> rng_euphoria -> roll( 0.01 * p -> talents.euphoria -> effect_base_value( 1 ) ) )
        {
          gain *= 2;
        }
        trigger_eclipse_energy_gain( this, gain );
      }
    }
  }

  virtual double execute_time() SC_CONST
  {
    druid_t* p = player -> cast_druid();
    if ( p -> buffs_t8_4pc_caster -> check() )
      return 0;

    return druid_spell_t::execute_time();
  }

  virtual bool ready()
  {
    if ( ! druid_spell_t::ready() )
      return false;

    druid_t* p = player -> cast_druid();

    if ( instant )
      if ( ! p -> buffs_t8_4pc_caster -> may_react() )
        return false;

    if ( extend_moonfire )
    {
      if ( ! p -> glyphs.starfire ) return false;
      if ( ! p -> dots_moonfire -> ticking() ) return false;
      if ( p -> dots_moonfire -> action -> added_ticks > 2 ) return false;
    }

    if ( ! prev_str.empty() )
    {
      if ( ! p -> last_foreground_action )
        return false;

      if ( p -> last_foreground_action -> name_str != prev_str )
        return false;
    }

    return true;
  }
};

// Wrath Spell ==============================================================

struct wrath_t : public druid_spell_t
{
  std::string prev_str;

  wrath_t( player_t* player, const std::string& options_str ) :
      druid_spell_t( "wrath", player, SCHOOL_NATURE, TREE_BALANCE )
  {
    druid_t* p = player -> cast_druid();

    option_t options[] =
    {
      { "prev",    OPT_STRING, &prev_str    },
      { NULL, OPT_UNKNOWN, NULL }
    };
    parse_options( options, options_str );

    id = 5176;
    parse_data( p -> player_data );

    may_crit          = true;
    
    // Times are given in ms in DBC files
    base_execute_time += p -> talents.starlight_wrath -> effect_base_value( 1 ) * 0.001;
    if ( p -> primary_tree() == TREE_BALANCE )
      base_crit_bonus_multiplier *= 1.0 + p -> spec_moonfury -> mod_additive( P_CRIT_DAMAGE );


    if ( p -> set_bonus.tier7_4pc_caster() ) base_crit += 0.05;
    if ( p -> set_bonus.tier9_4pc_caster() ) base_multiplier   *= 1.04;

  }

  virtual void player_buff()
  {
    druid_spell_t::player_buff();
    druid_t* p = player -> cast_druid();
    if ( p -> glyphs.wrath && p -> dots_insect_swarm -> ticking() )
      player_multiplier *= 1.10;
  }
  virtual void travel( int    travel_result,
                       double travel_dmg )
  {
    druid_t* p = player -> cast_druid();
    druid_spell_t::travel( travel_result, travel_dmg );
    if ( result_is_hit() )
    {
      if ( travel_result == RESULT_CRIT )
      {
        trigger_t10_4pc_caster( player, travel_dmg, SCHOOL_NATURE );
      }
      trigger_earth_and_moon( this );

      if ( ! p -> buffs_eclipse_lunar -> check() )
      {
        // effect#2 of wrath spell => 92679
        // 92679 -> base_value == 13
        // It actualy is 13.3333, SF and W have the exact same eclipse gain per second
        double gain = -(40.0/3.0); 
        if ( ! p -> buffs_eclipse_solar -> check() 
          && p -> rng_euphoria -> roll( 0.01 * p -> talents.euphoria -> effect_base_value( 1 ) ) )
        {
          gain *= 2;
        }
        trigger_eclipse_energy_gain( this, gain );
      }
    }
  }

  virtual bool ready()
  {
    if ( ! druid_spell_t::ready() )
      return false;

    druid_t* p = player -> cast_druid();

    if ( ! prev_str.empty() )
    {
      if ( ! p -> last_foreground_action )
        return false;

      if ( p -> last_foreground_action -> name_str != prev_str )
        return false;
    }

    return true;
  }
};

// Starfall Spell ===========================================================

struct starfall_t : public druid_spell_t
{
  spell_t* starfall_star;

  starfall_t( player_t* player, const std::string& options_str ) :
      druid_spell_t( "starfall", player, SCHOOL_ARCANE, TREE_BALANCE )
  {
    struct starfall_star_t : public druid_spell_t
    {
      starfall_star_t( player_t* player ) : druid_spell_t( "starfall", player, SCHOOL_ARCANE, TREE_BALANCE )
      {
        druid_t* p = player -> cast_druid();

        direct_power_mod = 0.30;
        may_crit         = true;
        may_miss         = true;
        may_resist       = true;
        background       = true;
        proc             = true;
        dual             = true;

        base_dd_min = base_dd_max  = 0;
        if ( p -> primary_tree() == TREE_BALANCE )
          base_crit_bonus_multiplier *= 1.0 + p -> spec_moonfury -> mod_additive( P_CRIT_DAMAGE );


        if ( p -> glyphs.focus )
          base_multiplier *= 1.1;

        id = 53195;
      }

      virtual void execute()
      {
        druid_spell_t::execute();
        tick_dmg = direct_dmg;
        update_stats( DMG_OVER_TIME );
      }
    };
    druid_t* p = player -> cast_druid();

    option_t options[] =
    {
      { NULL, OPT_UNKNOWN, NULL }
    };
    parse_options( options, options_str );

    static rank_t ranks[] =
    {
      { 80, 4, 563, 653, 0, 0.35 },
      { 75, 3, 474, 551, 0, 0.35 },
      { 70, 2, 324, 377, 0, 0.35 },
      { 60, 1, 114, 167, 0, 0.35 },
      { 0, 0, 0, 0, 0, 0 }
    };
    init_rank( ranks );

    num_ticks      = 10;
    base_tick_time = 1.0;

    cooldown -> duration          = 90;
    if ( p -> glyphs.starfall )
      cooldown -> duration  -= 30;

    base_execute_time = 0;

    may_miss = may_crit = false; // The spell only triggers the buff



    starfall_star = new starfall_star_t( p );
    starfall_star -> base_dd_max = base_dd_max;
    starfall_star -> base_dd_min = base_dd_min;
    base_dd_min = base_dd_max = 0;

  }
  virtual void tick()
  {
    if ( sim -> debug ) log_t::output( sim, "%s ticks (%d of %d)", name(), current_tick, num_ticks );
    starfall_star -> execute();
    update_time( DMG_OVER_TIME );
  }
};


// Starsurge Spell ==========================================================

struct starsurge_t : public druid_spell_t
{
  cooldown_t* starfall_cd;
  starsurge_t( player_t* player, const std::string& options_str ) :
      druid_spell_t( "starsurge", player, SCHOOL_SPELLSTORM, TREE_BALANCE ),
      starfall_cd( 0 )
  {
    druid_t* p = player -> cast_druid();
    
    check_spec( TREE_BALANCE );
    
    option_t options[] =
    {
      { NULL, OPT_UNKNOWN, NULL }
    };
    parse_options( options, options_str );

    id = 78674;
    parse_data( p -> player_data );

    if ( p -> primary_tree() == TREE_BALANCE )
      base_crit_bonus_multiplier *= 1.0 + p -> spec_moonfury -> mod_additive( P_CRIT_DAMAGE );
    
    starfall_cd = p -> get_cooldown( "starfall" );
  }
  
  virtual void travel( int    travel_result,
                       double travel_dmg )
  {
    druid_t* p = player -> cast_druid();
    druid_spell_t::travel( travel_result, travel_dmg );

    if ( travel_result == RESULT_CRIT || travel_result == RESULT_HIT )
    {
      // gain is positive for p -> eclipse_bar_direction==0
      // else it is towards p -> eclipse_bar_direction
      double gain = 15;
      if ( p -> eclipse_bar_direction < 0 ) gain = -gain;

      trigger_eclipse_energy_gain( this, gain );

      if ( p -> glyphs.starsurge )
        starfall_cd -> ready -= 5.0;
    }
  }

  virtual void schedule_execute()
  {
    druid_spell_t::schedule_execute();
    druid_t* p = player -> cast_druid();
    p -> buffs_shooting_stars -> expire();
  }
  
  virtual double execute_time() SC_CONST
  {
    druid_t* p = player -> cast_druid();
    if ( p -> buffs_shooting_stars -> up() )
      return 0;

    return druid_spell_t::execute_time();
  }  
};



// Typhoon Spell ============================================================

struct typhoon_t : public druid_spell_t
{
  typhoon_t( player_t* player, const std::string& options_str ) :
      druid_spell_t( "typhoon", player, SCHOOL_NATURE, TREE_BALANCE )
  {
    druid_t* p = player -> cast_druid();
    check_talent( p -> talents.typhoon -> rank() );
    option_t options[] =
    {
      { NULL, OPT_UNKNOWN, NULL }
    };
    parse_options( options, options_str );

    static rank_t ranks[] =
    {
      { 80, 5, 1190, 1190, 0, 0.25 },
      { 75, 4, 1010, 1010, 0, 0.25 },
      { 70, 3,  735,  735, 0, 0.25 },
      { 60, 2,  550,  550, 0, 0.25 },
      { 50, 1,  400,  400, 0, 0.25 },
      {  0, 0,    0,    0, 0, 0 }
    };
    init_rank( ranks );

    base_execute_time = 0;
    direct_power_mod  = 0.193;
    base_multiplier *= 1.0 + 0.15 * p -> talents.gale_winds -> rank();
    aoe = true;

    cooldown -> duration = 20;
    if ( p -> glyphs.monsoon )
      cooldown -> duration -= 3;
    if ( p -> glyphs.typhoon )
      base_cost *= 0.92;
  }
};

// Mark of the Wild Spell =====================================================

struct mark_of_the_wild_t : public druid_spell_t
{
  mark_of_the_wild_t( player_t* player, const std::string& options_str ) :
      druid_spell_t( "mark_of_the_wild", player, SCHOOL_NATURE, TREE_RESTORATION )
  {
    // TODO: Cata => +5% like BoK
    // druid_t* p = player -> cast_druid();

    trigger_gcd = 0;
    id = 1126;
  }

  virtual void execute()
  {
    if ( sim -> log ) log_t::output( sim, "%s performs %s", player -> name(), name() );

    for ( player_t* p = sim -> player_list; p; p = p -> next )
    {
      if ( p -> ooc_buffs() )
      {
        p -> buffs.mark_of_the_wild -> trigger();
      }
    }
  }

  virtual bool ready()
  {
    return ! player -> buffs.mark_of_the_wild -> check();
  }
};

// Treants Spell ============================================================

struct treants_spell_t : public druid_spell_t
{
  treants_spell_t( player_t* player, const std::string& options_str ) :
      druid_spell_t( "treants", player, SCHOOL_NATURE, TREE_BALANCE )
  {
    druid_t* p = player -> cast_druid();
    check_talent( p -> talents.force_of_nature -> rank() );

    option_t options[] =
    {
      { NULL, OPT_UNKNOWN, NULL }
    };
    parse_options( options, options_str );

    cooldown -> duration = 180.0;
    base_cost = p -> resource_base[ RESOURCE_MANA ] * 0.12;
    id = 33831;
  }

  virtual void execute()
  {
    consume_resource();
    update_ready();
    player -> summon_pet( "treants", 30.0 );
  }
};

// Stealth ==================================================================

struct stealth_t : public spell_t
{
  stealth_t( player_t* player, const std::string& options_str ) :
      spell_t( "stealth", player )
  {
    trigger_gcd = 0;
  }

  virtual void execute()
  {
    druid_t* p = player -> cast_druid();
    if ( sim -> log ) log_t::output( sim, "%s performs %s", p -> name(), name() );
    p -> buffs_stealthed -> trigger();
  }

  virtual bool ready()
  {
    druid_t* p = player -> cast_druid();
    return ! p -> buffs_stealthed -> check();
  }
};

} // ANONYMOUS NAMESPACE ===================================================

// ==========================================================================
// Druid Character Definition
// ==========================================================================

// druid_t::create_action  ==================================================

action_t* druid_t::create_action( const std::string& name,
                                  const std::string& options_str )
{
  if ( name == "auto_attack"       ) return new       auto_attack_t( this, options_str );
  if ( name == "bash"              ) return new              bash_t( this, options_str );
  if ( name == "berserk_bear"      ) return new      berserk_bear_t( this, options_str );
  if ( name == "berserk_cat"       ) return new       berserk_cat_t( this, options_str );
  if ( name == "bear_form"         ) return new         bear_form_t( this, options_str );
  if ( name == "cat_form"          ) return new          cat_form_t( this, options_str );
  if ( name == "claw"              ) return new              claw_t( this, options_str );
  if ( name == "enrage"            ) return new            enrage_t( this, options_str );
  if ( name == "faerie_fire"       ) return new       faerie_fire_t( this, options_str );
  if ( name == "faerie_fire_feral" ) return new faerie_fire_feral_t( this, options_str );
  if ( name == "ferocious_bite"    ) return new    ferocious_bite_t( this, options_str );
  if ( name == "insect_swarm"      ) return new      insect_swarm_t( this, options_str );
  if ( name == "innervate"         ) return new         innervate_t( this, options_str );
  if ( name == "lacerate"          ) return new          lacerate_t( this, options_str );
  if ( name == "maim"              ) return new              maim_t( this, options_str );
  if ( name == "mangle_bear"       ) return new       mangle_bear_t( this, options_str );
  if ( name == "mangle_cat"        ) return new        mangle_cat_t( this, options_str );
  if ( name == "mark_of_the_wild"  ) return new  mark_of_the_wild_t( this, options_str );
  if ( name == "maul"              ) return new              maul_t( this, options_str );
  if ( name == "moonfire"          ) return new          moonfire_t( this, options_str );
  if ( name == "moonkin_form"      ) return new      moonkin_form_t( this, options_str );
  if ( name == "natures_swiftness" ) return new  druids_swiftness_t( this, options_str );
  if ( name == "rake"              ) return new              rake_t( this, options_str );
  if ( name == "rip"               ) return new               rip_t( this, options_str );
  if ( name == "savage_roar"       ) return new       savage_roar_t( this, options_str );
  if ( name == "shred"             ) return new             shred_t( this, options_str );
  if ( name == "starfire"          ) return new          starfire_t( this, options_str );
  if ( name == "starfall"          ) return new          starfall_t( this, options_str );
  if ( name == "starsurge"         ) return new         starsurge_t( this, options_str );
  if ( name == "stealth"           ) return new           stealth_t( this, options_str );
  if ( name == "swipe_bear"        ) return new        swipe_bear_t( this, options_str );
  if ( name == "tigers_fury"       ) return new       tigers_fury_t( this, options_str );
  if ( name == "treants"           ) return new     treants_spell_t( this, options_str );
  if ( name == "typhoon"           ) return new           typhoon_t( this, options_str );
  if ( name == "wrath"             ) return new             wrath_t( this, options_str );
#if 0
  if ( name == "cower"             ) return new             cower_t( this, options_str );
  if ( name == "maim"              ) return new              maim_t( this, options_str );
  if ( name == "prowl"             ) return new             prowl_t( this, options_str );
  if ( name == "ravage"            ) return new            ravage_t( this, options_str );
  if ( name == "swipe_cat"         ) return new         swipe_cat_t( this, options_str );
#endif

  return player_t::create_action( name, options_str );
}

// druid_t::create_pet ======================================================

pet_t* druid_t::create_pet( const std::string& pet_name )
{
  pet_t* p = find_pet( pet_name );

  if ( p ) return p;

  if ( pet_name == "treants" ) return new treants_pet_t( sim, this, pet_name );

  return 0;
}

// druid_t::create_pets =====================================================

void druid_t::create_pets()
{
  create_pet( "treants" );
}

// druid_t::init_talents =====================================================

void druid_t::init_talents()
{
  talents.balance_of_power      = new talent_t( this, "balance_of_power", "Balance of Power" );
  talents.berserk               = new talent_t( this, "berserk", "Berserk" );
  talents.blessing_of_the_grove = new talent_t( this, "blessing_of_the_grove", "Blessing of the Grove" );
  talents.blood_in_the_water    = new talent_t( this, "blood_in_the_water", "Blood in the Water" );
  talents.brutal_impact         = new talent_t( this, "brutal_impact", "Brutal Impact" );
  talents.earth_and_moon        = new talent_t( this, "earth_and_moon", "Earth and Moon" );
  talents.endless_carnage       = new talent_t( this, "endless_carnage", "Endless Carnage" );
  talents.euphoria              = new talent_t( this, "euphoria", "Euphoria" );
  talents.dreamstate            = new talent_t( this, "dreamstate", "Dreamstate" );               
  talents.feral_aggression      = new talent_t( this, "feral_aggression", "Feral Aggression" );
  talents.feral_charge          = new talent_t( this, "feral_charge", "Feral Charge" );
  talents.feral_swiftness       = new talent_t( this, "feral_swiftness", "Feral Swiftness" );
  talents.force_of_nature       = new talent_t( this, "force_of_nature", "Force of Nature" );
  talents.fungal_growth         = new talent_t( this, "fungal_growth", "Fungal Growth" );
  talents.furor                 = new talent_t( this, "furor", "Furor" );
  talents.fury_of_stormrage     = new talent_t( this, "fury_of_stormrage", "Fury of Stormrage" );
  talents.fury_swipes           = new talent_t( this, "fury_swipes", "Fury Swipes" );
  talents.gale_winds            = new talent_t( this, "gale_winds", "Gale Winds" );
  talents.genesis               = new talent_t( this, "genesis", "Genesis" );
  talents.heart_of_the_wild     = new talent_t( this, "heart_of_the_wild", "Heart of the Wild" );
  talents.infected_wounds       = new talent_t( this, "infected_wounds", "Infected Wounds" );
  talents.king_of_the_jungle    = new talent_t( this, "king_of_the_jungle", "King of the Jungle" );
  talents.leader_of_the_pack    = new talent_t( this, "leader_of_the_pack", "Leader of the Pack" );
  talents.lunar_shower          = new talent_t( this, "lunar_shower", "Lunar Shower" );
  talents.master_shapeshifter   = new talent_t( this, "master_shapeshifter", "Master Shapeshifter" );
  talents.moonglow              = new talent_t( this, "moonglow", "Moonglow" );
  talents.moonkin_form          = new talent_t( this, "moonkin_form", "Moonkin Form" );
  talents.natural_reaction      = new talent_t( this, "natural_reaction", "Natural Reaction" );
  talents.natures_majesty       = new talent_t( this, "natures_majesty", "Nature's Majesty" );
  talents.natures_swiftness     = new talent_t( this, "natures_swiftness", "Nature's Swiftness" ); 
  talents.natures_grace         = new talent_t( this, "natures_grace", "Nature's Grace" );
  talents.nurturing_instict     = new talent_t( this, "nurturing_instinct", "Nurturing Instinct" );
//  talents.overgrowth            = new talent_t( this, "overgrowth", "Overgrowth" );
  talents.owlkin_frenzy         = new talent_t( this, "owlkin_frenzy", "Owlkin Frenzy" );
  talents.predatory_strikes     = new talent_t( this, "predatory_strikes", "Predatory Strikes" );
  talents.primal_fury           = new talent_t( this, "primal_fury", "Primal Fury" );
  talents.primal_madness        = new talent_t( this, "primal_madness", "Primal Madness" );
  talents.pulverize             = new talent_t( this, "pulverize", "Pulverize" );
  talents.rend_and_tear         = new talent_t( this, "rend_and_tear", "Rend and Tear" );
  talents.shooting_stars        = new talent_t( this, "shooting_stars", "Shooting Stars" );
  talents.solar_beam            = new talent_t( this, "solar_beam", "Solar Beam" );
  talents.stampede              = new talent_t( this, "stampede", "Stampede" );
  talents.starfall              = new talent_t( this, "starfall", "Starfall" );
  talents.starlight_wrath       = new talent_t( this, "starlight_wrath", "Starlight Wrath" );
  talents.sunfire               = new talent_t( this, "sunfire", "Sunfire" );
  talents.survival_instincts    = new talent_t( this, "survival_instincts", "Survival Instincts" );
  talents.thick_hide            = new talent_t( this, "thick_hide", "Thick Hide" );
  talents.typhoon               = new talent_t( this, "typhoon", "Typhoon" );

  player_t::init_talents();
}

// druid_t::init_spells =====================================================

void druid_t::init_spells()
{
  player_t::init_spells();

  // Balance
  spec_moonfury         = new passive_spell_t( this, "moonfury",      16913 );
  mastery_total_eclipse = new mastery_t      ( this, "total_eclipse", 77492, TREE_BALANCE );

  // Feral
  spec_aggression         = new passive_spell_t( this, "aggression",      84735 );
  spec_vengeance          = new passive_spell_t( this, "vengeance",       84840 );
  mastery_razor_claws     = new mastery_t      ( this, "razor_claws",     77493, TREE_FERAL );
  mastery_savage_defender = new mastery_t      ( this, "savage_defender", 77494, TREE_FERAL );
}

// druid_t::init_glyphs =====================================================

void druid_t::init_glyphs()
{
  memset( ( void* ) &glyphs, 0x0, sizeof( glyphs_t ) );

  std::vector<std::string> glyph_names;
  int num_glyphs = util_t::string_split( glyph_names, glyphs_str, ",/" );

  for ( int i=0; i < num_glyphs; i++ )
  {
    std::string& n = glyph_names[ i ];

    if      ( n == "berserk"               ) glyphs.berserk = 1;
    else if ( n == "ferocious_bite"        ) glyphs.ferocious_bite = 1;
    else if ( n == "focus"                 ) glyphs.focus = 1;
    else if ( n == "innervate"             ) glyphs.innervate = 1;
    else if ( n == "insect_swarm"          ) glyphs.insect_swarm = 1;
    else if ( n == "lacerate"              ) glyphs.lacerate = 1;
    else if ( n == "mangle"                ) glyphs.mangle = 1;
    else if ( n == "monsoon"               ) glyphs.monsoon = 1;
    else if ( n == "moonfire"              ) glyphs.moonfire = 1;
    else if ( n == "rip"                   ) glyphs.rip = 1;
    else if ( n == "savage_roar"           ) glyphs.savage_roar = 1;
    else if ( n == "shred"                 ) glyphs.shred = 1;
    else if ( n == "starfall"              ) glyphs.starfall = 1;
    else if ( n == "starfire"              ) glyphs.starfire = 1;
    else if ( n == "starsurge"             ) glyphs.starsurge = 1;
    else if ( n == "tigers_fury"           ) glyphs.tigers_fury = 1;
    else if ( n == "typhoon"               ) glyphs.typhoon = 1;
    else if ( n == "wrath"                 ) glyphs.wrath = 1;
    // minor glyphs, to prevent 'not-found' warning
    else if ( n == "aquatic_form"          ) ;
    else if ( n == "barkskin"              ) ;
    else if ( n == "challenging_roar"      ) ;
    else if ( n == "dash"                  ) ;
    else if ( n == "entangling_roots"      ) ;
    else if ( n == "faerie_fire"           ) ;
    else if ( n == "feral_charge"          ) ;
    else if ( n == "frenzied_regeneration" ) ;
    else if ( n == "healing_touch"         ) ;
    else if ( n == "hurricane"             ) ;
    else if ( n == "lifebloom"             ) ;
    else if ( n == "mark_of_the_wild"      ) ;
    else if ( n == "maul"                  ) ;
    else if ( n == "rake"                  ) ;
    else if ( n == "rebirth"               ) ;
    else if ( n == "regrowth"              ) ;
    else if ( n == "rejuvenation"          ) ;
    else if ( n == "solar_beam"            ) ;
    else if ( n == "swiftmend"             ) ;
    else if ( n == "thorns"                ) ;
    else if ( n == "unburdened_rebirth"    ) ;
    else if ( n == "wild_growth"           ) ;
    else if ( ! sim -> parent ) 
    {
      sim -> errorf( "Player %s has unrecognized glyph %s\n", name(), n.c_str() );
    }
  }
}

// druid_t::init_race ======================================================

void druid_t::init_race()
{
  race = util_t::parse_race_type( race_str );
  switch ( race )
  {
  case RACE_NIGHT_ELF:
  case RACE_TAUREN:
  case RACE_WORGEN:
  case RACE_TROLL:
    break;
  default:
    race = RACE_NIGHT_ELF;
    race_str = util_t::race_type_string( race );
  }

  player_t::init_race();
}

// druid_t::init_base =======================================================

void druid_t::init_base()
{
  player_t::init_base();

  initial_spell_power_per_spirit = 0.0;

  base_attack_power = -20 + level * (level > 80 ? 3.0 : 2.0);

  attribute_multiplier_initial[ ATTR_INTELLECT ]   *= 1.0 + talents.heart_of_the_wild -> effect_base_value( 1 ) * 0.01;
  initial_attack_power_per_agility  = 0.0;
  initial_attack_power_per_strength = 2.0;

  // FIXME! Level-specific!  Should be form-specific!
  base_miss    = 0.05;
  initial_armor_multiplier  = 1.0 + util_t::talent_rank( talents.thick_hide -> rank(), 3, 0.04, 0.07, 0.10 );

  diminished_kfactor    = 0.009720;
  diminished_dodge_capi = 0.008555;
  diminished_parry_capi = 0.008555;

  resource_base[ RESOURCE_ENERGY ] = 100;
  resource_base[ RESOURCE_RAGE   ] = 100;
  mana_per_intellect      = 15;
  health_per_stamina      = 10;
  energy_regen_per_second = 10;
  
  // Furor: +5/10/15% max mana
  resource_base[ RESOURCE_MANA ] *= 1.0 + talents.furor -> effect_base_value( 2 ) * 0.01;
  mana_per_intellect             *= 1.0 + talents.furor -> effect_base_value( 2 ) * 0.01;
  
  switch ( primary_tree() )
  {
  case TREE_BALANCE:
    // Dreamstate for choosing balance
    // 12857 Dreamstate seems to be gone
    // mp5_per_intellect = 0.10;
    break;
  case TREE_RESTORATION:
    // Intensity for choosing resto
    mana_regen_while_casting = 0.50;
    break;
  case TREE_FERAL:
    break;
  default:
    break;
  }

  base_gcd = 1.5;

  //if ( tank == -1 && talents.is_tank_spec() -> rank ) tank = 1;
}

// druid_t::init_buffs ======================================================

void druid_t::init_buffs()
{
  player_t::init_buffs();

  // buff_t( sim, player, name, max_stack, duration, cooldown, proc_chance, quiet )
  buffs_berserk            = new buff_t( this, "berserk"           , 1,  15.0 + ( glyphs.berserk ? 5.0 : 0.0 ) );
  buffs_eclipse_lunar      = new buff_t( this, "lunar_eclipse"     , 1 );
  buffs_eclipse_solar      = new buff_t( this, "solar_eclipse"     , 1 );
  buffs_enrage             = new buff_t( this, "enrage"            , 1,  10.0 );
  buffs_lacerate           = new buff_t( this, "lacerate"          , 3,  15.0 );
  buffs_lunar_shower       = new buff_t( this, "lunar_shower"      , 3,   3.0,     0, talents.lunar_shower -> rank() );
  buffs_natures_grace      = new buff_t( this, "natures_grace"     , 1,  15.0,  60.0, talents.natures_grace -> rank() );
  buffs_natures_swiftness  = new buff_t( this, "natures_swiftness" , 1, 180.0, 180.0 );
  buffs_omen_of_clarity    = new buff_t( this, "omen_of_clarity"   , 1,  15.0,     0, 3.5 / 60.0 );
  buffs_pulverize          = new buff_t( this, "pulverize"         , 1,  10.0 + 4.0 * talents.endless_carnage -> rank() );
  buffs_shooting_stars     = new buff_t( this, "shooting_stars"    , 1,   8.0,     0, talents.shooting_stars -> rank() * 0.02 );
  buffs_t8_4pc_caster      = new buff_t( this, "t8_4pc_caster"     , 1,  10.0,     0, set_bonus.tier8_4pc_caster() * 0.08 );
  buffs_t10_2pc_caster     = new buff_t( this, "t10_2pc_caster"    , 1,   6.0,     0, set_bonus.tier10_2pc_caster() );
  buffs_t11_4pc_caster     = new buff_t( this, "t11_4pc_caster"    , 3,   8.0,     0, set_bonus.tier11_4pc_caster() );

  buffs_tigers_fury        = new buff_t( this, "tigers_fury"       , 1,   6.0 );
  buffs_glyph_of_innervate = new buff_t( this, "glyph_of_innervate", 1,  10.0,     0, glyphs.innervate);
  
  // stat_buff_t( sim, player, name, stat, amount, max_stack, duration, cooldown, proc_chance, quiet )

  // simple
  buffs_bear_form    = new buff_t( this, "bear_form" );
  buffs_cat_form     = new buff_t( this, "cat_form" );
  buffs_combo_points = new buff_t( this, "combo_points", 5 );
  buffs_moonkin_form = new buff_t( this, "moonkin_form" );
  buffs_savage_roar  = new buff_t( this, "savage_roar" );
  buffs_stealthed    = new buff_t( this, "stealthed" );
}

// druid_t::init_items ======================================================

void druid_t::init_items()
{
  player_t::init_items();

}

// druid_t::init_scaling ====================================================

void druid_t::init_scaling()
{
  player_t::init_scaling();

  equipped_weapon_dps = main_hand_weapon.damage / main_hand_weapon.swing_time;

  scales_with[ STAT_WEAPON_SPEED  ] = 0;
}

// druid_t::init_gains ======================================================

void druid_t::init_gains()
{
  player_t::init_gains();

  gains_bear_melee         = get_gain( "bear_melee"         );
  gains_energy_refund      = get_gain( "energy_refund"      );
  gains_enrage             = get_gain( "enrage"             );
  gains_euphoria           = get_gain( "euphoria"           );
  gains_glyph_of_innervate = get_gain( "glyph_of_innervate" );
  gains_incoming_damage    = get_gain( "incoming_damage"    );
  gains_moonkin_form       = get_gain( "moonkin_form"       );
  gains_natural_reaction   = get_gain( "natural_reaction"   );
  gains_omen_of_clarity    = get_gain( "omen_of_clarity"    );
  gains_primal_fury        = get_gain( "primal_fury"        );
  gains_tigers_fury        = get_gain( "tigers_fury"        );
}

// druid_t::init_procs ======================================================

void druid_t::init_procs()
{
  player_t::init_procs();

  procs_combo_points_wasted = get_proc( "combo_points_wasted" );
  procs_fury_swipes         = get_proc( "fury_swipes"         );
  procs_parry_haste         = get_proc( "parry_haste"         );
  procs_primal_fury         = get_proc( "primal_fury"         );
  procs_tier8_2pc           = get_proc( "tier8_2pc"           );
}

// druid_t::init_uptimes ====================================================

void druid_t::init_uptimes()
{
  player_t::init_uptimes();

  uptimes_energy_cap = get_uptime( "energy_cap" );
  uptimes_rage_cap   = get_uptime( "rage_cap"   );
  uptimes_rip        = get_uptime( "rip"        );
  uptimes_rake       = get_uptime( "rake"       );
}

// druid_t::init_rng ========================================================

void druid_t::init_rng()
{
  player_t::init_rng();

  rng_euphoria        = get_rng( "euphoria"       );
  rng_fury_swipes     = get_rng( "fury_swipes"    );
  rng_blood_in_the_water  = get_rng( "blood_in_the_water" );
  rng_primal_fury     = get_rng( "primal_fury"    );
}

// druid_t::init_actions ====================================================

void druid_t::init_actions()
{
  if ( primary_role() == ROLE_ATTACK && main_hand_weapon.type == WEAPON_NONE )
  {
    sim -> errorf( "Player %s has no weapon equipped at the Main-Hand slot.", name() );
    quiet = true;
    return;
  }

  if ( action_list_str.empty() )
  {
    std::string use_str = "";
    int num_items = ( int ) items.size();
    for ( int i=0; i < num_items; i++ )
    {
      if ( items[ i ].use.active() )
      {
        use_str += "/use_item,name=";
        use_str += items[ i ].name();
      }
    }

    if ( primary_tree() == TREE_FERAL )
    {
      if ( tank > 0 )
      {
        action_list_str += "flask,type=endless_rage/food,type=rhinolicious_wormsteak";
        action_list_str += "/bear_form";
        action_list_str += "/auto_attack";
        action_list_str += "/snapshot_stats";
        action_list_str += "/faerie_fire_feral,debuff_only=1";  // Use on pull.
        action_list_str += "/mangle_bear,mangle<=0.5";
        action_list_str += "/lacerate,lacerate<=6.9";           // This seems to be the sweet spot to prevent Lacerate falling off.
        if ( talents.berserk -> rank() ) action_list_str+="/berserk_bear";
        action_list_str += use_str;
        action_list_str += "/mangle_bear";
        action_list_str += "/faerie_fire_feral";
        action_list_str += "/swipe_bear,berserk=0,lacerate_stack>=5";
      }
      else
      {
        action_list_str += "flask,type=endless_rage";
        action_list_str += "/food,type=hearty_rhino";
        action_list_str += "/cat_form";
        action_list_str += "/speed_potion,if=!in_combat|buff.bloodlust.react|target.time_to_die<=60";
        action_list_str += "/auto_attack";
        action_list_str += "/snapshot_stats";
        action_list_str += "/maim";
        action_list_str += "/faerie_fire_feral,debuff_only=1";
        action_list_str += "/tigers_fury,if=energy<=30&!buff.berserk.up";
        if ( talents.berserk -> rank() )action_list_str += "/berserk_cat,if=energy>=80&energy<=90&!buff.tigers_fury.up";
        action_list_str += "/savage_roar,if=buff.combo_points.stack>=1&buff.savage_roar.remains<=1";
        action_list_str += "/rip,if=buff.combo_points.stack>=5&target.time_to_die>=6";
        action_list_str += "/savage_roar,if=buff.combo_points.stack>=3&target.time_to_die>=9&buff.savage_roar.remains<=8&dot.rip.remains-buff.savage_roar.remains>=-3";
        action_list_str += "/ferocious_bite,if=target.time_to_die<=6&buff.combo_points.stack>=5";
        action_list_str += "/ferocious_bite,if=target.time_to_die<=1&buff.combo_points.stack>=4";
        action_list_str += "/ferocious_bite,if=buff.combo_points.stack>=5&dot.rip.remains>=8&buff.savage_roar.remains>=11";
        if ( glyphs.shred )action_list_str += "/shred,extend_rip=1,if=dot.rip.remains<=4";
        action_list_str += "/mangle_cat,mangle<=1";
        action_list_str += "/rake,if=target.time_to_die>=9";
        action_list_str += "/shred,if=(buff.combo_points.stack<=4|dot.rip.remains>=0.8)&dot.rake.remains>=0.4&(energy>=80|buff.omen_of_clarity.react|dot.rip.remains<=2|buff.berserk.up|cooldown.tigers_fury.remains<=3)";
        action_list_str += "/shred,if=target.time_to_die<=9";
        action_list_str += "/shred,if=buff.combo_points.stack<=0&buff.savage_roar.remains<=2";
      }
    }
    else
    {
      action_list_str += "flask,type=frost_wyrm/food,type=fish_feast/mark_of_the_wild";
      if ( talents.moonkin_form -> rank() ) 
        action_list_str += "/moonkin_form";
      action_list_str += "/snapshot_stats";
      action_list_str += "/speed_potion,if=!in_combat|(buff.bloodlust.react&buff.lunar_eclipse.react)|(target.time_to_die<=60&buff.lunar_eclipse.react)";
      if ( talents.typhoon -> rank() ) 
        action_list_str += "/typhoon,moving=1";
      action_list_str += "/innervate,trigger=-2000";
      if ( talents.force_of_nature -> rank() )
        action_list_str+="/treants,time>=5";
      if ( talents.starfall -> rank() ) 
        action_list_str+="/starfall,if=!eclipse";
      action_list_str += "/starfire,if=buff.t8_4pc_caster.up";
      action_list_str += "/moonfire,if=!ticking";
      action_list_str += "/insect_swarm,if=!ticking";
      action_list_str += "/starsurge";
      action_list_str += use_str;
      action_list_str += "/starfire,if=eclipse_dir=-1";
      action_list_str += "/wrath,if=eclipse_dir=1";
      action_list_str += "/starfire";
    }
    action_list_default = 1;
  }

  player_t::init_actions();

}

// druid_t::reset ===========================================================

void druid_t::reset()
{
  player_t::reset();

  eclipse_bar_value = 0;
  base_gcd = 1.5;
}

// druid_t::interrupt =======================================================

void druid_t::interrupt()
{
  player_t::interrupt();

  if ( main_hand_attack ) main_hand_attack -> cancel();
}

// druid_t::clear_debuffs ===================================================

void druid_t::clear_debuffs()
{
  player_t::clear_debuffs();

  buffs_combo_points -> expire();
}

// druid_t::regen ===========================================================

void druid_t::regen( double periodicity )
{
  player_t::regen( periodicity );

  int resource_type = primary_resource();

  if ( resource_type == RESOURCE_ENERGY )
  {
    uptimes_energy_cap -> update( resource_current[ RESOURCE_ENERGY ] ==
                                  resource_max    [ RESOURCE_ENERGY ] );
  }
  else if ( resource_type == RESOURCE_MANA)
  {
    if ( buffs_glyph_of_innervate -> check() )
      resource_gain( RESOURCE_MANA, buffs_glyph_of_innervate -> value() * periodicity, gains_glyph_of_innervate );
  }
  else if ( resource_type == RESOURCE_RAGE )
  {
    if ( buffs_enrage -> up() ) resource_gain( RESOURCE_RAGE, 1.0 * periodicity, gains_enrage );

    uptimes_rage_cap -> update( resource_current[ RESOURCE_RAGE ] ==
                                resource_max    [ RESOURCE_RAGE ] );
  }
}

// druid_t::available =======================================================

double druid_t::available() SC_CONST
{
  if ( primary_resource() != RESOURCE_ENERGY ) return 0.1;

  double energy = resource_current[ RESOURCE_ENERGY ];

  if ( energy > 25 ) return 0.1;

  return std::max( ( 25 - energy ) / energy_regen_per_second, 0.1 );
}

// druid_t::composite_attack_power ==========================================

double druid_t::composite_attack_power() SC_CONST
{
  double ap = player_t::composite_attack_power();

  if ( buffs_cat_form  -> check() )
  {
      ap += 2.0 * ( agility() - 10.0 );
  }

  return floor( ap );
}

// druid_t::composite_attack_power_multiplier ===============================

double druid_t::composite_attack_power_multiplier() SC_CONST
{
  double multiplier = player_t::composite_attack_power_multiplier();

  if ( buffs_cat_form -> check() )
  {
    multiplier *= 1.0 + talents.heart_of_the_wild -> effect_base_value( 2 ) * 0.01;
  }
  if ( primary_tree() == TREE_FERAL )
  {
    // Aggression
    // Increases your attack power by 25%.
    multiplier *= 1.25;
  }
  return multiplier;
}

// druid_t::composite_attack_crit ==========================================

double druid_t::composite_attack_crit() SC_CONST
{
  double c = player_t::composite_attack_crit();

  if ( buffs_cat_form -> check() )
  {
    c += 0.04 * talents.master_shapeshifter -> rank();
  }

  return floor( c * 10000.0 ) / 10000.0;
}

// druid_t::composite_spell_hit =============================================

double druid_t::composite_spell_hit() SC_CONST
{
  double hit = player_t::composite_spell_hit();
  // BoP does not convert base spirit into hit!
  hit += ( spirit() - attribute_base[ ATTR_SPIRIT ] ) * ( 0.01 * talents.balance_of_power -> effect_base_value( 2 ) ) / rating.spell_hit;

  return hit;
}

// druid_t::composite_spell_crit ============================================

double druid_t::composite_spell_crit() SC_CONST
{
  double crit = player_t::composite_spell_crit();
  crit += talents.natures_majesty -> rank() * 0.02;
  return crit;
}

// druid_t::composite_attribute_multiplier ==================================

double druid_t::composite_attribute_multiplier( int attr ) SC_CONST
{
  double m = player_t::composite_attribute_multiplier( attr );

  // The matching_gear_multiplier is done statically for performance reasons,
  // unfortunately that's before we're in cat form or bear form, so let's compensate here
  if ( attr == ATTR_STAMINA && buffs_bear_form -> check() )
  {
    m *= 1.0 + talents.heart_of_the_wild -> effect_base_value( 2 ) * 0.01;
    if (primary_tree() == TREE_FERAL)
      m *= 1.05;
  }
  else if ( attr == ATTR_AGILITY && buffs_cat_form -> check() )
  {
    if (primary_tree() == TREE_FERAL)
      m *= 1.05;
  }

  return m;
}

// druid_t::matching_gear_multiplier ==================================

double druid_t::matching_gear_multiplier( const attribute_type attr ) SC_CONST
{
  switch ( primary_tree() )
  {
  case TREE_BALANCE:
  case TREE_RESTORATION:
    if ( attr == ATTR_INTELLECT )
      return 0.05;
    break;
  default:
    break;
  }

  return 0.0;
}

// druid_t::composite_tank_crit =============================================

double druid_t::composite_tank_crit( const school_type school ) SC_CONST
{
  double c = player_t::composite_tank_crit( school );

  if ( school == SCHOOL_PHYSICAL )
  {
    c -= 0.02 * talents.thick_hide -> rank();
    if ( c < 0 ) c = 0;
  }

  return c;
}

// druid_t::create_expression ===============================================

action_expr_t* druid_t::create_expression( action_t* a, const std::string& name_str )
{
  if ( name_str == "eclipse" )
  {
    struct eclipse_expr_t : public action_expr_t
    {
      eclipse_expr_t( action_t* a) : action_expr_t( a, "eclipse", TOK_NUM ) {}
      virtual int evaluate() { result_num = action -> player -> cast_druid() -> eclipse_bar_value; return TOK_NUM; }
    };
    return new eclipse_expr_t( a );
  }
  else if ( name_str == "eclipse_dir" )
  {
    struct eclipse_dir_expr_t : public action_expr_t
    {
      eclipse_dir_expr_t( action_t* a) : action_expr_t( a, "eclipse_dir", TOK_NUM ) {}
      virtual int evaluate() { result_num = action -> player -> cast_druid() -> eclipse_bar_direction; return TOK_NUM; }
    };
    return new eclipse_dir_expr_t( a );
  }
  return player_t::create_expression( a, name_str );
}

// druid_t::get_talent_trees ===============================================

std::vector<talent_translation_t>& druid_t::get_talent_list()
{
  talent_list.clear();
  return talent_list;
}

// druid_t::get_options ================================================

std::vector<option_t>& druid_t::get_options()
{
  if ( options.empty() )
  {
    player_t::get_options();

    option_t druid_options[] =
    {
      // @option_doc loc=player/druid/talents title="Talents"
      // @option_doc loc=player/druid/misc title="Misc"
      { NULL, OPT_UNKNOWN, NULL }
    };

    option_t::copy( options, druid_options );
  }

  return options;
}

// druid_t::decode_set ======================================================

int druid_t::decode_set( item_t& item )
{
  if ( item.slot != SLOT_HEAD      &&
       item.slot != SLOT_SHOULDERS &&
       item.slot != SLOT_CHEST     &&
       item.slot != SLOT_HANDS     &&
       item.slot != SLOT_LEGS      )
  {
    return SET_NONE;
  }

  const char* s = item.name();

  bool is_caster = ( strstr( s, "cover"     ) ||
                     strstr( s, "mantle"    ) ||
                     strstr( s, "vestment" ) ||
                     strstr( s, "trousers"  ) ||
                     strstr( s, "gloves"    ) );

  bool is_melee = ( strstr( s, "headguard"    ) ||
                    strstr( s, "shoulderpads" ) ||
                    strstr( s, "raiment"      ) ||
                    strstr( s, "legguards"    ) ||
                    strstr( s, "handgrips"    ) );

  if ( strstr( s, "dreamwalker" ) )
  {
    if ( is_caster ) return SET_T7_CASTER;
    if ( is_melee  ) return SET_T7_MELEE;
  }
  if ( strstr( s, "nightsong" ) )
  {
    if ( is_caster ) return SET_T8_CASTER;
    if ( is_melee  ) return SET_T8_MELEE;
  }
  if ( strstr( s, "malfurions" ) ||
       strstr( s, "runetotems" ) )
  {
    if ( is_caster ) return SET_T9_CASTER;
    if ( is_melee  ) return SET_T9_MELEE;
  }
  if ( strstr( s, "lasherweave" ) )
  {
    if ( is_caster ) return SET_T10_CASTER;
    if ( is_melee  ) return SET_T10_MELEE;
  }
  if ( strstr( s, "stormrider" ) )
  {
    bool is_caster = ( strstr( s, "cover"         ) ||
                       strstr( s, "shoulderwraps" ) ||
                       strstr( s, "vestment"      ) ||
                       strstr( s, "leggings"      ) ||
                       strstr( s, "gloves"        ) );

    bool is_melee = ( strstr( s, "headpiece"    ) ||
                      strstr( s, "spaulders"    ) ||
                      strstr( s, "raiment"      ) ||
                      strstr( s, "legguards"    ) ||
                      strstr( s, "grips"        ) );
    if ( is_caster ) return SET_T11_CASTER;
    if ( is_melee  ) return SET_T11_MELEE;
  }

  return SET_NONE;
}

// druid_t::primary_role ====================================================

int druid_t::primary_role() SC_CONST
{
  switch ( primary_tree() )
  {
  case TREE_BALANCE:     
  case TREE_RESTORATION:
    return ROLE_SPELL;
  case TREE_FERAL:
    return ROLE_ATTACK;
  default:
    return ROLE_NONE;
  }
}

// druid_t::primary_resource ================================================

int druid_t::primary_resource() SC_CONST
{
  if ( talents.moonkin_form -> rank() ) return RESOURCE_MANA;
  if ( tank > 0 ) return RESOURCE_RAGE;
  return RESOURCE_ENERGY;
}

// druid_t::target_swing ====================================================

int druid_t::target_swing()
{
  int result = player_t::target_swing();

  if ( sim -> log ) log_t::output( sim, "%s swing result: %s", sim -> target -> name(), util_t::result_type_string( result ) );

  if ( result == RESULT_HIT    ||
       result == RESULT_CRIT   ||
       result == RESULT_GLANCE ||
       result == RESULT_BLOCK  )
  {
    resource_gain( RESOURCE_RAGE, 100.0, gains_incoming_damage );  // FIXME! Assume it caps rage every time.
  }
  if ( result == RESULT_DODGE )
  {
    if ( talents.natural_reaction -> rank() )
    {
      resource_gain( RESOURCE_RAGE, talents.natural_reaction -> rank(), gains_natural_reaction );
    }
  }
  if ( result == RESULT_PARRY )
  {
    if ( main_hand_attack && main_hand_attack -> execute_event )
    {
      double swing_time = main_hand_attack -> time_to_execute;
      double max_reschedule = ( main_hand_attack -> execute_event -> occurs() - 0.20 * swing_time ) - sim -> current_time;

      if ( max_reschedule > 0 )
      {
        main_hand_attack -> reschedule_execute( std::min( ( 0.40 * swing_time ), max_reschedule ) );
        procs_parry_haste -> occur();
      }
    }
  }
  return result;
}

// ==========================================================================
// PLAYER_T EXTENSIONS
// ==========================================================================

// player_t::create_druid  ==================================================

player_t* player_t::create_druid( sim_t*             sim,
                                  const std::string& name,
                                  race_type r )
{
  return new druid_t( sim, name, r );
}

// player_t::druid_init =====================================================

void player_t::druid_init( sim_t* sim )
{
  sim -> auras.leader_of_the_pack = new aura_t( sim, "leader_of_the_pack" );
  sim -> auras.moonkin            = new aura_t( sim, "moonkin" );

  for ( player_t* p = sim -> player_list; p; p = p -> next )
  {
    p -> buffs.innervate        = new buff_t( p, "innervate",        1, 10.0 );
    p -> buffs.mark_of_the_wild = new buff_t( p, "mark_of_the_wild", !p -> is_pet() );
  }

  for ( target_t* t = sim -> target_list; t; t = t -> next )
  {
    t -> debuffs.earth_and_moon       = new debuff_t( t, "earth_and_moon",       1,  12.0 );
    t -> debuffs.faerie_fire          = new debuff_t( t, "faerie_fire",          3, 300.0 );
    t -> debuffs.infected_wounds      = new debuff_t( t, "infected_wounds",      1,  12.0 );
    t -> debuffs.mangle               = new debuff_t( t, "mangle",               1,  60.0 );
  }
}

// player_t::druid_combat_begin =============================================

void player_t::druid_combat_begin( sim_t* sim )
{
  if ( sim -> overrides.leader_of_the_pack     ) sim -> auras.leader_of_the_pack -> override();
  if ( sim -> overrides.moonkin_aura           ) sim -> auras.moonkin            -> override();

  for ( player_t* p = sim -> player_list; p; p = p -> next )
  {
    if ( p -> ooc_buffs() )
    {
      if ( sim -> overrides.mark_of_the_wild ) p -> buffs.mark_of_the_wild -> override();
    }
  }

  for ( target_t* t = sim -> target_list; t; t = t -> next )
  {
    if ( sim -> overrides.earth_and_moon       ) t -> debuffs.earth_and_moon       -> override( 1, 8 );
    if ( sim -> overrides.faerie_fire          ) t -> debuffs.faerie_fire          -> override( 3, 0.04 );
    if ( sim -> overrides.infected_wounds      ) t -> debuffs.infected_wounds      -> override();
    if ( sim -> overrides.mangle               ) t -> debuffs.mangle               -> override();
  }
}

